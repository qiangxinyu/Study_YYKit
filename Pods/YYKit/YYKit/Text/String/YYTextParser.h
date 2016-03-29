//
//  YYTextParser.h
//  YYKit <https://github.com/ibireme/YYKit>
//
//  Created by ibireme on 15/3/6.
//  Copyright (c) 2015 ibireme.
//
//  This source code is licensed under the MIT-style license found in the
//  LICENSE file in the root directory of this source tree.
//

#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

/**
 The YYTextParser protocol declares the required method for YYTextView and YYLabel
 to modify the text during editing.
 
 You can implement this protocol to add code highlighting or emoticon replacement for
 YYTextView and YYLabel. See `YYTextSimpleMarkdownParser` and `YYTextSimpleEmoticonParser` for example.
 */
@protocol YYTextParser <NSObject>
@required
/**
 When text is changed in YYTextView or YYLabel, this method will be called.
 当改变的文本是yytextview或yylabel，这个方法会被调用。
 
 @param text  The original attributed string. This method may parse the text and
 change the text attributes or content.
 原来的 string   这个方法可以分析文本和更改文本属性或内容。
 
 @param selectedRange  Current selected range in `text`.
 This method should correct the range if the text content is changed. If there's 
 no selected range (such as YYLabel), this value is NULL.
 当前选择的范围`文本`。
 如果改变文本内容，该方法应正确的范围,这个方法可以返回 一个范围 
 没有选择的范围（如yylabel），这个值是null。
 如果“文本”在该方法中被修改，返回“是”，否则返回“不”。
 @return If the 'text' is modified in this method, returns `YES`, otherwise returns `NO`.
 */
- (BOOL)parseText:(nullable NSMutableAttributedString *)text selectedRange:(nullable NSRangePointer)selectedRange;
@end



/**
 A simple markdown parser.
 
 It'a very simple markdown parser, you can use this parser to highlight some 
 small piece of markdown text.
 
 This markdown parser use regular expression to parse text, slow and weak.
 If you want to write a better parser, try these projests:
 https://github.com/NimbusKit/markdown
 https://github.com/dreamwieber/AttributedMarkdown
 https://github.com/indragiek/CocoaMarkdown
 
 Or you can use lex/yacc to generate your custom parser.
 */
@interface YYTextSimpleMarkdownParser : NSObject <YYTextParser>
@property (nonatomic) CGFloat fontSize;         ///< default is 14
@property (nonatomic) CGFloat headerFontSize;   ///< default is 20

@property (nullable, nonatomic, strong) UIColor *textColor;
@property (nullable, nonatomic, strong) UIColor *controlTextColor;
@property (nullable, nonatomic, strong) UIColor *headerTextColor;
@property (nullable, nonatomic, strong) UIColor *inlineTextColor;
@property (nullable, nonatomic, strong) UIColor *codeTextColor;
@property (nullable, nonatomic, strong) UIColor *linkTextColor;

- (void)setColorWithBrightTheme; ///< reset the color properties to pre-defined value.
- (void)setColorWithDarkTheme;   ///< reset the color properties to pre-defined value.
@end



/**
 A simple emoticon parser.
 
 Use this parser to map some specified piece of string to image emoticon.
 Example: "Hello :smile:"  ->  "Hello 😀"
 
 It can also be used to extend the "unicode emoticon".
 */
@interface YYTextSimpleEmoticonParser : NSObject <YYTextParser>

/**
 The custom emoticon mapper.
 The key is a specified plain string, such as @":smile:".
 The value is a UIImage which will replace the specified plain string in text.
 */
@property (nullable, copy) NSDictionary<NSString *, __kindof UIImage *> *emoticonMapper;
@end

NS_ASSUME_NONNULL_END
