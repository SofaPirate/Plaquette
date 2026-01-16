#!/bin/bash
# Post-process Doxygen XML to remove pq:: namespace prefix from class names
# This makes the documentation more beginner-friendly

XML_DIR="${1:-xml}"

if [ ! -d "$XML_DIR" ]; then
    echo "Error: XML directory '$XML_DIR' not found"
    exit 1
fi

# Detect OS for sed in-place compatibility
if [[ "$OSTYPE" == "darwin"* ]]; then
    SED_INPLACE="sed -i ''"
else
    SED_INPLACE="sed -i"
fi

# Strip pq:: prefix from compound names and references in all XML files
for f in "$XML_DIR"/*.xml; do
    $SED_INPLACE \
        -e 's/<name>pq::\([^<]*\)<\/name>/<name>\1<\/name>/g' \
        -e 's/<compoundname>pq::\([^<]*\)<\/compoundname>/<compoundname>\1<\/compoundname>/g' \
        -e 's/refid="classpq_1_1_/refid="class/g' \
        "$f"
done

# Rename class XML files to remove pq_1_1_ prefix
for f in "$XML_DIR"/classpq_1_1_*.xml; do
    if [ -f "$f" ]; then
        newname=$(echo "$f" | sed 's/classpq_1_1_/class/')
        mv "$f" "$newname"
    fi
done

echo "Namespace pq:: stripped from XML files in $XML_DIR"
