//
//  YYLabel.h
//  YYKit <https://github.com/ibireme/YYKit>
//
//  Created by ibireme on 15/2/25.
//  Copyright (c) 2015 ibireme.
//
//  This source code is licensed under the MIT-style license found in the
//  LICENSE file in the root directory of this source tree.
//

#import <UIKit/UIKit.h>

#if __has_include(<YYKit/YYKit.h>)
#import <YYKit/YYTextParser.h>
#import <YYKit/YYTextLayout.h>
#import <YYKit/YYTextAttribute.h>
#else
#import "YYTextParser.h"
#import "YYTextLayout.h"
#import "YYTextAttribute.h"
#endif

NS_ASSUME_NONNULL_BEGIN

#if !TARGET_INTERFACE_BUILDER

/**
 The YYLabel class implements a read-only text view.
 
 @discussion The API and behavior is similar to UILabel, but provides more features:
 
 * It supports asynchronous layout and rendering (to avoid blocking UI thread).
 * It extends the CoreText attributes to support more text effects.
 * It allows to add UIImage, UIView and CALayer as text attachments.
 * It allows to add 'highlight' link to some range of text to allow user interact with.
 * It allows to add container path and exclusion paths to control text container's shape.
 * It supports vertical form layout to display CJK text.
 
 See NSAttributedString+YYText.h for more convenience methods to set the attributes.
 See YYTextAttribute.h and YYTextLayout.h for more information.
 */
@interface YYLabel : UIView <NSCoding>

#pragma mark - Accessing the Text Attributes
///=============================================================================
/// @name Accessing the Text Attributes
///=============================================================================

/**
 The text displayed by the label. Default is nil.
 Set a new value to this property also replaces the text in `attributedText`.
 Get the value returns the plain text in `attributedText`.
 
 标签的文本 默认为nil
 设置一个新的值 会替换掉 attributedText的 text
 get 会获取到 attributedText 的字符串
 
 */
@property (nullable, nonatomic, copy) NSString *text;

/**
 The font of the text. Default is 17-point system font.
 Set a new value to this property also causes the new font to be applied to the entire `attributedText`.
 Get the value returns the font at the head of `attributedText`.
 
 text的字体  默认是 17
 设置一个新值会应用到   attributedText
 get 也可以获取到 attributedText 的 font
 
 */
@property (null_resettable, nonatomic, strong) UIFont *font;

/**
 The color of the text. Default is black.
 Set a new value to this property also causes the new color to be applied to the entire `attributedText`.
 Get the value returns the color at the head of `attributedText`.
 
 text的颜色  默认是 黑色
 设置一个新值会应用到   attributedText
 get 也可以获取到 attributedText 的 颜色
 
 */
@property (null_resettable, nonatomic, strong) UIColor *textColor;

/**
 The shadow color of the text. Default is nil.
 Set a new value to this property also causes the shadow color to be applied to the entire `attributedText`.
 Get the value returns the shadow color at the head of `attributedText`.
 
 text的阴影颜色  默认是 nil
 设置一个新值会应用到   attributedText
 get 也可以获取到 attributedText 的 颜色
 
 */
@property (nullable, nonatomic, strong) UIColor *shadowColor;

/**
 The shadow offset of the text. Default is CGSizeZero.
 Set a new value to this property also causes the shadow offset to be applied to the entire `attributedText`.
 Get the value returns the shadow offset at the head of `attributedText`.
 
 text的阴影大小  默认是 CGSizeZero
 设置一个新值会应用到   attributedText
 get 也可以获取到 attributedText 的 颜色
 
 */
@property (nonatomic) CGSize shadowOffset;

/**
 The shadow blur of the text. Default is 0.
 Set a new value to this property also causes the shadow blur to be applied to the entire `attributedText`.
 Get the value returns the shadow blur at the head of `attributedText`.
 
 text的阴影模糊度  默认是 0
 设置一个新值会应用到   attributedText
 get 也可以获取到 attributedText 的 颜色
 */
@property (nonatomic) CGFloat shadowBlurRadius;

/**
 The technique to use for aligning the text. Default is NSLeftTextAlignment.
 Set a new value to this property also causes the new alignment to be applied to the entire `attributedText`.
 Get the value returns the alignment at the head of `attributedText`.
 
 文本的对齐方式，默认是 左对齐
 设置一个新值会应用到   attributedText
 get 也可以获取到 attributedText 的 颜色
 */
@property (nonatomic) NSTextAlignment textAlignment;

/**
 The text vertical aligmnent in container. Default is YYTextVerticalAlignmentCenter.
 文本的 垂直对准   默认是 中间
 */
@property (nonatomic) YYTextVerticalAlignment textVerticalAlignment;

/**
 The styled text displayed by the label.
 Set a new value to this property also replaces the value of the `text`, `font`, `textColor`,
 `textAlignment` and other properties in label.
 
 @discussion It only support the attributes declared in CoreText and YYTextAttribute.
 See `NSAttributedString+YYText` for more convenience methods to set the attributes.
 
 富文本
 设置一个新值 会应用到   text  font textColor textAlignment 和一些其他属性
 
 
 
 */
@property (nullable, nonatomic, copy) NSAttributedString *attributedText;

/**
 The technique to use for wrapping and truncating the label's text.
 Default is NSLineBreakByTruncatingTail.
 
 NSLineBreakByWordWrapping = 0,
 以单词为单位换行，以单位为单位截断。
 NSLineBreakByCharWrapping,
 以字符为单位换行，以字符为单位截断。
 NSLineBreakByClipping,
 以单词为单位换行。以字符为单位截断。
 NSLineBreakByTruncatingHead,
 以单词为单位换行。如果是单行，则开始部分有省略号。如果是多行，则中间有省略号，省略号后面有4个字符。
 NSLineBreakByTruncatingTail,
 以单词为单位换行。无论是单行还是多行，都是末尾有省略号。
 NSLineBreakByTruncatingMiddle,
 以单词为单位换行。无论是单行还是多行，都是中间有省略号，省略号后面只有2个字符。
 */
@property (nonatomic) NSLineBreakMode lineBreakMode;

/**
 The truncation token string used when text is truncated. Default is nil.
 When the value is nil, the label use "…" as default truncation token.
 */
@property (nullable, nonatomic, copy) NSAttributedString *truncationToken;

/**
 The maximum number of lines to use for rendering text. Default value is 1.
 0 means no limit.
 */
@property (nonatomic) NSUInteger numberOfLines;

/**
 When `text` or `attributedText` is changed, the parser will be called to modify the text.
 It can be used to add code highlighting or emoticon replacement to text view. 
 The default value is nil.
 当 text 或者 attributedText 改变, 这个 解析器 将调用来修改文本，
  它可以用来添加代码高亮或表情符号替换文本视图。
 
 See `YYTextParser` protocol for more information.
 */
@property (nullable, nonatomic, strong) id<YYTextParser> textParser;

/**
 The current text layout in text view. It can be used to query the text layout information.
 Set a new value to this property also replaces most properties in this label, such as `text`,
 `color`, `attributedText`, `lineBreakMode`, `textContainerPath`, `exclusionPaths` and so on.
 
 一个文本中的 当前布局， 可以用来查询文本布局信息。
 设置一个新值 也会应用到 label 中的多个属性，比如 `text`,
 `color`, `attributedText`, `lineBreakMode`, `textContainerPath`, `exclusionPaths` 等
 */
@property (nullable, nonatomic, strong) YYTextLayout *textLayout;


#pragma mark - Configuring the Text Container
#pragma mark - 配置             文本 容器
///=============================================================================
/// @name Configuring the Text Container
///=============================================================================

/**
 A UIBezierPath object that specifies the shape of the text frame. Default value is nil.
 一个 UIBezierPath 对象 在 text 的 frame 中指定一个形状， 默认是 nil
 */
@property (nullable, nonatomic, copy) UIBezierPath *textContainerPath;

/**
 An array of UIBezierPath objects representing the exclusion paths inside the
 receiver's bounding rectangle. Default value is nil.
 一个 UIBezierPath 对象的 数组，用来表示 矩形内的一堆形状
 */
@property (nullable, nonatomic, copy) NSArray<UIBezierPath *> *exclusionPaths;

/**
 The inset of the text container's layout area within the text view's content area.
 Default value is UIEdgeInsetsZero.
 一个text内容的 约束
 */
@property (nonatomic) UIEdgeInsets textContainerInset;

/**
 Whether the receiver's layout orientation is vertical form. Default is NO.
 It may used to display CJK text.
 接收机的布局定位是否垂直的形式。默认是否定的。
 它可以用来显示CJK文本。
 */
@property (nonatomic, getter=isVerticalForm) BOOL verticalForm;

/**
 The text line position modifier used to modify the lines' position in layout.
 用于修改布局中的线路位置的文本行位置调整。
 Default value is nil.
 See `YYTextLinePositionModifier` protocol for more information.
 
 */
@property (nullable, nonatomic, copy) id<YYTextLinePositionModifier> linePositionModifier;

/**
 The debug option to display CoreText layout result.
 The default value is [YYTextDebugOption sharedDebugOption].
 调试选项显示的一些布局的结果。
 默认值是[ yytextdebugoption shareddebugoption ]。
 */
@property (nullable, nonatomic, copy) YYTextDebugOption *debugOption;


#pragma mark - Getting the Layout Constraints
#pragma mark - 获得布局约束
///=============================================================================
/// @name Getting the Layout Constraints
///=============================================================================

/**
 The preferred maximum width (in points) for a multiline label.
 
 @discussion This property affects the size of the label when layout constraints 
     are applied to it. During layout, if the text extends beyond the width 
     specified by this property, the additional text is flowed to one or more new 
     lines, thereby increasing the height of the label. If the text is vertical 
     form, this value will match to text height.
 
 首选的最大宽度（分）为多行标签。
 “讨论这个属性会影响布局约束时的大小
 适用于它。在布局中，如果文本超出了宽度
 此属性所指定的附加文本将流到一个或多个新的
 线，从而增加了标签的高度。如果文本是垂直的
 形式，这个值将与文本高度匹配。
 */
@property (nonatomic) CGFloat preferredMaxLayoutWidth;


#pragma mark - Interacting with Text Data
///=============================================================================
/// @name Interacting with Text Data
///=============================================================================

/**
 When user tap the label, this action will be called (similar to tap gesture).
 The default value is nil.
 当用户点击这个action标签（类似于所谓的点击动作）。
 默认值是零。
 */
@property (nullable, nonatomic, copy) YYTextAction textTapAction;

/**
 When user long press the label, this action will be called (similar to long press gesture).
 The default value is nil.
 当用户长按标签时，这个动作将被称为（类似于长按手势）。
 默认值为零。
 */
@property (nullable, nonatomic, copy) YYTextAction textLongPressAction;

/**
 When user tap the highlight range of text, this action will be called.
 The default value is nil.
 当用户点击文本的高亮显示范围时，这个动作将被调用。
 默认值为零。
 */
@property (nullable, nonatomic, copy) YYTextAction highlightTapAction;

/**
 When user long press the highlight range of text, this action will be called.
 The default value is nil. 
 当用户长按文本的高亮范围时，这个动作将被调用。
 默认值为零。
 */
@property (nullable, nonatomic, copy) YYTextAction highlightLongPressAction;



#pragma mark - Configuring the Display Mode
#pragma mark - Configuring the Display Mode
///=============================================================================
/// @name Configuring the Display Mode
///=============================================================================

/**
 A Boolean value indicating whether the layout and rendering codes are running
 asynchronously on background threads. 
 
 The default value is `NO`.
 
 一个布尔值，该值指示布局和渲染代码是否在运行后台线程异步。
 默认值是“不”。
 */
@property (nonatomic) BOOL displaysAsynchronously;

/**
 If the value is YES, and the layer is rendered asynchronously, then it will
 set label.layer.contents to nil before display. 
 
 The default value is `YES`.
 
 @discussion When the asynchronously display is enabled, the layer's content will
 be updated after the background render process finished. If the render process
 can not finished in a vsync time (1/60 second), the old content will be still kept
 for display. You may manually clear the content by set the layer.contents to nil 
 after you update the label's properties, or you can just set this property to YES.
 如果该值是是的，并且该层是异步的，那么它将
 集label.layer.contents零显示前。
 默认值是“是”。
 当异步显示启用时，该层的内容将被
 在后台渲染处理完成后进行更新。如果渲染过程
 不能在垂直同步完成（1 / 60秒），旧的内容仍将保持
 用于显示。你可以手动设置layer.contents零清除内容
 更新标签的属性后，或者您可以将此属性设置为“是”。
 */
@property (nonatomic) BOOL clearContentsBeforeAsynchronouslyDisplay;

/**
 If the value is YES, and the layer is rendered asynchronously, then it will add 
 a fade animation on layer when the contents of layer changed. 
 
 The default value is `YES`.
 如果该值是是的，并且该层是异步的，那么它将增加
 层变化时的一个渐变动画。
 默认值是“是”。
 */
@property (nonatomic) BOOL fadeOnAsynchronouslyDisplay;

/**
 If the value is YES, then it will add a fade animation on layer when some range
 of text become highlighted. 
 
 The default value is `YES`.
 
 如果值是肯定的，那么在一定范围内，它会在图层上添加一个渐变的动画
 文字变得突出。
 默认值是“是”。
 */
@property (nonatomic) BOOL fadeOnHighlight;

/**
 Ignore common properties (such as text, font, textColor, attributedText...) and
 only use "textLayout" to display content. 
 
 The default value is `NO`.
 
 @discussion If you control the label content only through "textLayout", then
 you may set this value to YES for higher performance.
 忽视共同的属性（如文本、字体、文字颜色、attributedtext…）和
 用“、”来显示内容。
 默认值是“不”。
 @讨论如果你控制标签的内容只有通过“、”，然后
 您可以将此值设置为“是的更高性能。
 */
@property (nonatomic) BOOL ignoreCommonProperties;

/*
 Tips:
 
 1. If you only need a UILabel alternative to display rich text and receive link touch event, 
    you do not need to adjust the display mode properties.
 如果你只需要一个UILabel替代显示丰富的文字和接收链路的触摸事件，
 您不需要调整显示模式属性。
 
 2. If you have performance issues, you may enable the asynchronous display mode
    by setting the `displaysAsynchronously` to YES.
 如果您有性能问题，您可以启用异步显示模式
 通过设置` displaysasynchronously `YES。
 
 3. If you want to get the highest performance, you should do text layout with
    `YYTextLayout` class in background thread. Here's an example:
 如果你想获得最高的性能，你应该做文本布局
 ` yytextlayout `后台线程类。下面是一个例子：
    
    YYLabel *label = [YYLabel new];
    label.displaysAsynchronously = YES;
    label.ignoreCommonProperties = YES;
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
 
        // Create attributed string.
        NSMutableAttributedString *text = [[NSMutableAttributedString alloc] initWithString:@"Some Text"];
        text.font = [UIFont systemFontOfSize:16];
        text.color = [UIColor grayColor];
        [text setColor:[UIColor redColor] range:NSMakeRange(0, 4)];
 
        // Create text container
        YYTextContainer *container = [YYTextContainer new];
        container.size = CGSizeMake(100, CGFLOAT_MAX);
        container.maximumNumberOfRows = 0;
        
        // Generate a text layout.
        YYTextLayout *layout = [YYTextLayout layoutWithContainer:container text:text];
        
        dispatch_async(dispatch_get_main_queue(), ^{
            label.size = layout.textBoundingSize;
            label.textLayout = layout;
        });
    });
 
 */

@end


#else // TARGET_INTERFACE_BUILDER
IB_DESIGNABLE
@interface YYLabel : UIView <NSCoding>
@property (nullable, nonatomic, copy) IBInspectable NSString *text;
@property (null_resettable, nonatomic, strong) IBInspectable UIColor *textColor;
@property (nullable, nonatomic, strong) IBInspectable NSString *fontName_;
@property (nonatomic) IBInspectable CGFloat fontSize_;
@property (nonatomic) IBInspectable BOOL fontIsBold_;
@property (nonatomic) IBInspectable NSUInteger numberOfLines;
@property (nonatomic) IBInspectable NSInteger lineBreakMode;
@property (nonatomic) IBInspectable CGFloat preferredMaxLayoutWidth;
@property (nonatomic, getter=isVerticalForm) IBInspectable BOOL verticalForm;
@property (nonatomic) IBInspectable NSInteger textAlignment;
@property (nonatomic) IBInspectable NSInteger textVerticalAlignment;
@property (nullable, nonatomic, strong) IBInspectable UIColor *shadowColor;
@property (nonatomic) IBInspectable CGPoint shadowOffset;
@property (nonatomic) IBInspectable CGFloat shadowBlurRadius;
@property (nullable, nonatomic, copy) IBInspectable NSAttributedString *attributedText;
@property (nonatomic) IBInspectable CGFloat insetTop_;
@property (nonatomic) IBInspectable CGFloat insetBottom_;
@property (nonatomic) IBInspectable CGFloat insetLeft_;
@property (nonatomic) IBInspectable CGFloat insetRight_;
@property (nonatomic) IBInspectable BOOL debugEnabled_;

@property (null_resettable, nonatomic, strong) UIFont *font;
@property (nullable, nonatomic, copy) NSAttributedString *truncationToken;
@property (nullable, nonatomic, strong) id<YYTextParser> textParser;
@property (nullable, nonatomic, strong) YYTextLayout *textLayout;
@property (nullable, nonatomic, copy) UIBezierPath *textContainerPath;
@property (nullable, nonatomic, copy) NSArray<UIBezierPath *> *exclusionPaths;
@property (nonatomic) UIEdgeInsets textContainerInset;
@property (nullable, nonatomic, copy) id<YYTextLinePositionModifier> linePositionModifier;
@property (nonnull, nonatomic, copy) YYTextDebugOption *debugOption;
@property (nullable, nonatomic, copy) YYTextAction textTapAction;
@property (nullable, nonatomic, copy) YYTextAction textLongPressAction;
@property (nullable, nonatomic, copy) YYTextAction highlightTapAction;
@property (nullable, nonatomic, copy) YYTextAction highlightLongPressAction;
@property (nonatomic) BOOL displaysAsynchronously;
@property (nonatomic) BOOL clearContentsBeforeAsynchronouslyDisplay;
@property (nonatomic) BOOL fadeOnAsynchronouslyDisplay;
@property (nonatomic) BOOL fadeOnHighlight;
@property (nonatomic) BOOL ignoreCommonProperties;
@end
#endif // !TARGET_INTERFACE_BUILDER

NS_ASSUME_NONNULL_END
