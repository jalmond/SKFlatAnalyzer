#!/bin/bash
if [ -z "$SKFlatV" ];then
    echo "Please source setup.sh"
    exit 1
fi

if [ -z "$1" ]; then
    echo "Usage: $0 SEARCHDIR [dry]"
    echo "Example: $0 /gv0/DATA/SKFlat/$SKFlatV"
    exit 1
fi

SEARCHDIR=$(realpath $1)
if [ ! -z "$2" ];then
    echo "Dry-run..."
    DRY=true
fi

## find /gv0/DATA/SKFlat/Run2UltraLegacy_v1/2017/DATA/SingleElectron/periodB/210120_042549/ -type f |sort -V > $SKFlat_WD/data/Run2UltraLegacy_v1/2017/Sample/ForSNU/
while read line <&3; do
    array=(${line//\// })

    ## find SKFlatV position
    for i in $(seq "${#array[@]}"); do
	if [ "${array[$i]}" = "$SKFlatV" ]; then
	    VI=$i ## SKFlatV position index
	    break
	fi
    done
    ## get common info
    YEAR="${array[$(($VI+1))]}"
    TYPE="${array[$(($VI+2))]}"
    if [[ "$TYPE" = *"DATA"* ]]; then
	## continue for subdirectories
	test "${#array[@]}" -eq "$(($VI+5))" || continue
	## get info for DATA
	SAMPLE="${array[$(($VI+3))]}"
	PERIOD="${array[$(($VI+4))]}"
	ALIAS="${SAMPLE}_${PERIOD//period/}"
    elif [[ "$TYPE" = *"MC"* ]]; then
	## continue for subdirectories
	test "${#array[@]}" -eq "$(($VI+4))" || continue
	## get info for MC
	SAMPLE="${array[$(($VI+3))]}"
	ALIAS=$(grep -P "$SAMPLE\t" $SKFlat_WD/data/$SKFlatV/$YEAR/Sample/SampleSummary_MC.txt|awk '{print $1}'|head -n1)
	if [ "$ALIAS" = "" ]; then
	    ALIAS=$(cat $SKFlat_WD/data/$SKFlatV/*/Sample/SampleSummary_MC.txt|grep -P "$SAMPLE\t"|awk '{print $1}'|head -n1)
	fi 
	if [ "$ALIAS" = "" ]; then
	    cat $SKFlat_WD/data/$SKFlatV/*/Sample/SampleSummary_Signal_Type1.txt|grep -P "$SAMPLE\t"|awk '{print $1}'|head -n1
            ALIAS=$(cat $SKFlat_WD/data/$SKFlatV/*/Sample/SampleSummary_Signal_Type1.txt|grep -P  "$SAMPLE\t"|awk '{print $1}'|head -n1)
        fi
	if [ "$ALIAS" = "" ]; then
	    ALIAS=$(cat $SKFlat_WD/data/*/*/Sample/SampleSummary_MC.txt|grep -P "$SAMPLE\t"|awk '{print $1}'|head -n1)
	fi  
	if [ "$ALIAS" = "" ]; then
	    read -p "cannot find alias for $SAMPLE. exit or insert alias: " ALIAS
	fi
    else continue;
    fi

    
    echo "ALIAS="$ALIAS
    SKIM=$(echo $TYPE|grep -o "SkimTree_.*$")
    
    NTIMESTAMP=$(ls $line|sort -V|wc -l)
    if [ "$NTIMESTAMP" -eq 0 ];then
	continue
    elif [ "$NTIMESTAMP" -eq 1 ]; then
	TIMESTAMP=$(ls $line|sort -V|tail -n1)
    elif [ "$NTIMESTAMP" -gt 1 ]; then
	echo "Multiple timestamps at $line:"
	echo "  $(ls $line|sort -V|awk 'BEGIN{ORS=" "}{print}')"
	TIMESTAMP=$(ls $line|sort -V|tail -n1)
    fi
	
    SOURCE=$line/$TIMESTAMP
    TARGET=$SKFlat_WD/data/$SKFlatV/$YEAR/Sample/ForSNU/$ALIAS.txt
    [ -n "$SKIM" ] && TARGET=$SKFlat_WD/data/$SKFlatV/$YEAR/Sample/ForSNU/${SKIM}_$ALIAS.txt

    if [ -f $TARGET ]; then
	diffout="$(diff $TARGET <(find $SOURCE -type f|sort -V))"
	if [ $(echo "$diffout"|wc -l) -gt 1 ]; then
	    diff -u <(tail -n5 $TARGET) <(find $SOURCE -type f|sort -V|tail -n5)
	else continue
	fi
    fi
    echo "find $SOURCE -type f|sort -V > $TARGET"
    find $SOURCE -type f|sort -V > $TARGET

done 3< <(find $SEARCHDIR -type d|grep $SKFlatV|sort -V)
#done 3< <(find $SEARCHDIR -type d|grep $SKFlatV|sort -V|egrep -v "TypeI|private|sherpa")
