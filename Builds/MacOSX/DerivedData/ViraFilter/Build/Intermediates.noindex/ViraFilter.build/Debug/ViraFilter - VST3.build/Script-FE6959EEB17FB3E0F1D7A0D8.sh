#!/bin/sh
set -e
rm -f "$CONFIGURATION_BUILD_DIR/$FULL_PRODUCT_NAME/Contents/moduleinfo.json"
xcrun codesign --verify "$CONFIGURATION_BUILD_DIR/$FULL_PRODUCT_NAME" || xcrun codesign -f -s - "$CONFIGURATION_BUILD_DIR/$FULL_PRODUCT_NAME"
"$CONFIGURATION_BUILD_DIR/juce_vst3_helper" -create -version "1.0.0" -path "$CONFIGURATION_BUILD_DIR/$FULL_PRODUCT_NAME" -output "$CONFIGURATION_BUILD_DIR/$FULL_PRODUCT_NAME/Contents/Resources/moduleinfo.json"

