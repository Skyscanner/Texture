/*
 *  Copyright (c) 2014-present, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#import <UIKit/UIKit.h>

#ifndef ComponentKit_ASTextKitAttributes_h
#define ComponentKit_ASTextKitAttributes_h

@protocol ASTextKitTruncating;

extern NSString *const ASTextKitTruncationAttributeName;
/**
 Use ASTextKitEntityAttribute as the value of this attribute to embed a link or other interactable content inside the
 text.
 */
extern NSString *const ASTextKitEntityAttributeName;

static inline BOOL _objectsEqual(id<NSObject> obj1, id<NSObject> obj2)
{
  return obj1 == obj2 ? YES : [obj1 isEqual:obj2];
}

/**
 All NSObject values in this struct should be copied when passed into the TextComponent.
 */
struct ASTextKitAttributes {
  /**
   The string to be drawn.  ASTextKit will not augment this string with default colors, etc. so this must be complete.
   */
  NSAttributedString *attributedString;
  /**
   The string to use as the truncation string, usually just "...".  If you have a range of text you would like to
   restrict highlighting to (for instance if you have "... Continue Reading", use the ASTextKitTruncationAttributeName
   to mark the specific range of the string that should be highlightable.
   */
  NSAttributedString *truncationAttributedString;
  /**
   This is the character set that ASTextKit should attempt to avoid leaving as a trailing character before your
   truncation token.  By default this set includes "\s\t\n\r.,!?:;" so you don't end up with ugly looking truncation
   text like "Hey, this is some fancy Truncation!\n\n...".  Instead it would be truncated as "Hey, this is some fancy
   truncation...".  This is not always possible.

   Set this to the empty charset if you want to just use the "dumb" truncation behavior.  A nil value will be
   substituted with the default described above.
   */
  NSCharacterSet *avoidTailTruncationSet;
  /**
   The line-break mode to apply to the text.  Since this also impacts how TextKit will attempt to truncate the text
   in your string, we only support NSLineBreakByWordWrapping and NSLineBreakByCharWrapping.
   */
  NSLineBreakMode lineBreakMode;
  /**
   The maximum number of lines to draw in the drawable region.  Leave blank or set to 0 to define no maximum.
   */
  NSUInteger maximumNumberOfLines;
  /**
   An array of UIBezierPath objects representing the exclusion paths inside the receiver's bounding rectangle. Default value: nil.
   */
  NSArray *exclusionPaths;
  /**
   The shadow offset for any shadows applied to the text.  The coordinate space for this is the same as UIKit, so a
   positive width means towards the right, and a positive height means towards the bottom.
   */
  CGSize shadowOffset;
  /**
   The color to use in drawing the text's shadow.
   */
  UIColor *shadowColor;
  /**
   The opacity of the shadow from 0 to 1.
   */
  CGFloat shadowOpacity;
  /**
   The radius that should be applied to the shadow blur.  Larger values mean a larger, more blurred shadow.
   */
  CGFloat shadowRadius;
  /**
   The minimum scale that the textnode can apply to fit long words in constrained size.
   */
  CGFloat minimumScaleFactor;
  /**
   A pointer to a function that that returns a custom layout manager subclass. If nil, defaults to NSLayoutManager.
   */
  NSLayoutManager *(*layoutManagerFactory)(void);

  /**
   We provide an explicit copy function so we can use aggregate initializer syntax while providing copy semantics for
   the NSObjects inside.
   */
  const ASTextKitAttributes copy() const
  {
    return {
      [attributedString copy],
      [truncationAttributedString copy],
      [avoidTailTruncationSet copy],
      lineBreakMode,
      maximumNumberOfLines,
      [exclusionPaths copy],
      shadowOffset,
      [shadowColor copy],
      shadowOpacity,
      shadowRadius,
      minimumScaleFactor,
      layoutManagerFactory
    };
  };

  bool operator==(const ASTextKitAttributes &other) const
  {
    // These comparisons are in a specific order to reduce the overall cost of this function.
    return lineBreakMode == other.lineBreakMode
    && maximumNumberOfLines == other.maximumNumberOfLines
    && shadowOpacity == other.shadowOpacity
    && shadowRadius == other.shadowRadius
    && minimumScaleFactor == other.minimumScaleFactor
    && layoutManagerFactory == other.layoutManagerFactory
    && CGSizeEqualToSize(shadowOffset, other.shadowOffset)
    && _objectsEqual(exclusionPaths, other.exclusionPaths)
    && _objectsEqual(avoidTailTruncationSet, other.avoidTailTruncationSet)
    && _objectsEqual(shadowColor, other.shadowColor)
    && _objectsEqual(attributedString, other.attributedString)
    && _objectsEqual(truncationAttributedString, other.truncationAttributedString);
  }

  size_t hash() const;
};

#endif
