# HOW TO USE THIS SCRIPT:
# FROM THE MAIN FOLDER OF POSITRON, CALL FOR IT IN THE SCRIPTS FOLDER, PASSING THE NUMBER OF NODES AS ARGUMENT, SUCH AS
#
# $ bash scripts/change-input.sh 60nodes
#
# OR
#
# $ bash scripts/change-input.sh 150nodes
#
# VALID OPTIONS OF ARGUMENTS: 60nodes, 90nodes, 120nodes, 150nodes, 180nodes

numberofnodes=$1

options='30nodes 60nodes 90nodes 120nodes 150nodes 180nodes'

if [[ " $options " =~ .*\ $numberofnodes\ .* ]]; then
    cd scratch/
    rm input.yaml
    ln -s ./$numberofnodes/input.yaml input.yaml  
else
    echo "invalid option"
fi

