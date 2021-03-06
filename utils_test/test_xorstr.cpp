#include "test_settings.hpp"

#ifdef ENABLE_TESTS
#include "../utils_lib/external/doctest.hpp"

#include "../utils_lib/utils_xorstring.hpp"
#include "../utils_lib/utils_traits.hpp"

#if defined(UTILS_COMPILER_MSVC)
    #define ori_str \
        "e|f@,b#b4?[a:()~6a8b?<~)e'bb5,ce'<.8.^]#970:]?=1]=66!)d<:*6:2?2';d}{&&!@" \
        "}.$2|c&)%d:c:(4[/:c,6#)1)_]:%;da_>{&#~^>>6?5+]++3#>3?:/*{]]a4/[=(|[52)>4"
#else
    #define ori_str \
        "e|f@,b#b4?[a:()~6a8b?<~)e'bb5,ce'<.8.^]#970:]?=1]=66!)d<:*6:2?2';d}{&&!@" \
        "}.$2|c&)%d:c:(4[/:c,6#)1)_]:%;da_>{&#~^>>6?5+]++3#>3?:/*{]]a4/[=(|[52)>4" \
        "8{:?4!:4;c(@8*8|:3&@%<>3~=0<4^)2^;.(d?;6*/}[.@;.,,%f[]$|);+[>1',0&{>96[{" \
        "~=_[2248?>>&2@5(b1~}'db2/=,{)#_b$e62*/'7>~40_:6_|=~_5>f(^.5*102d{&_75'f&" \
        ">4|}7).e>&+=~)9+&>8(1;7_@1=7|',]<?=,@9&&f|#%'0*~]@+?f?'=e)fc=:*e8^!,=/@}" \
        "4_a%0f)[|+_!]|_7]'9{c3ae6[63]#@c+%@>8~:f0$#|_d]=&>3|$()5~50+4^2:5)/+4e[&" \
        "e:9<3~e)|}d>*+9[/?)_%+/&7(9%{a82_%#.5@ad{#1d0</0%:>:@#(5_^@[)'&:5]b7~(:)" \
        "3*<8]:[%5f%^aa54+7*(126;5&}}_2+>5/8?(1[6a?(0849+a_32&~]d=,fe8c@{ab;}@9([" \
        "a40e>b.|d)84{a1e}_+>19,+#*^~:4^}36%])b6c4@e+^1.8?4;f!^|9:_2b!:<[!$^'&9a5" \
        ".@?;%:$%~c*;?|0c)d=[ee~1}>.d:;!6.e/01d_+f621.7af%*e,{ca36?{3/ed=>)%&2+ee" \
        "4[([/.|#=;/&d/_':37{1@05'8>=:40];9&e0:3;+04c$(|/95=1;9+%0b'c##f1]~).{/?/" \
        "{,60'~31=||{:7.$##0@]=>!!<@3?'0@(d+%;e|}:d%c0.f|)!./+;.,7a<6=d^{~>c8@';$" \
        "0f6/f$]1.:+;_@9)'?/?48@6>{@5eca=:#},9];5;+}}|:;f)%2$,>72.&c_5@%0'(>(}7({" \
        "c6=%,_.&)_+4^+9,&>f),}.2f9,b$~.{b[+*]}!&::74d,#e;/?,*d8:[a&;#?e2e5]9[ab;" \
        "$.9a>0):/?$*?c5>2^~]]4>]e#*80#|:9^,c,!7,|?]2>8%<f53#0:[]*752]>7c=6|[b;4~" \
        "$&::7e/$*+[{{+^{|*,50{.?3(c+}~&0,+27=<d0f'7,>,&[#*.|a?>|&6&4;$:?.[e4797)" \
        "+=e^{@[c<!]c4;80c=d5/$*]be';]+%&)517#417*|>*5*%'7#9&(7e+.}=,30a}(.|+~{0a" \
        "d^>b{2*:)_!^{;'>41)^))b0~0&2?#*;}2:./21:6398$d?#.@/8d7|e~da+!4~/=2:'<e!;" \
        ",<+%,&&%@/_0^0[c_'](c<}_e*)8&#~28f2*.(?]&0@!4%6a).8d~~?f|/]|%|#9_'@$[b&#" \
        ">}@<5>8&;<<~6]b4|7,_%0~38?~2#e[||+(4&52=@>,|a''51&]d@e'%40{[5!)43*/~=^6[" \
        "b&3c@221^5@4.,/'$2._1e4#{,7}c<f(.)/*5f7]}2/)!'*7(%&3^<,6<b=88=1]~?|+3,}@" \
        "'%b0~;6*74]&|&$!}&2_&d+*,5[+(_/2&33%1f5({$2_15|c6~!<f58>23[fee;'?84,31!!" \
        "e.(d_'a4^>/<2.41*8'?._][9{[<8)2</<'&,4/1(3f;*)77)*5}f$&.*'5=!#{}95|^d*bd" \
        ">}e+..$!)1|)ed{8]2a;,$6;{b#:|(1{$#[4^a|69*{d&@>:~8)?e>7?[b4|}+[{5554$.^%" \
        "#0[@@2]29/1a{;5a*.7:c5{~[.c7!#&*%b]>?@,'d>2b+%9,!))(9d/_9bf:*:&{a4&@^a':" \
        "+^;_]#:e?*+1b.|&~0.$df6,,2?]]}e;#'+6.5<#'91{3[|8@:{b0/{);a})6>?5]f%{;'4e" \
        "e46f%*6/c.;&4/7]{*@4c|0~;>(b+!2]?%3]!0,75[~&d;{.8{@#{4[c]<88&5===51{0)5*" \
        "6+1;>1<<^2b8+}bf].e^]'$,+.]%)(8b%37b!a[2@,?9*!dd#d^|9!33}};db4,%080,(4@*" \
        "''|7+[{e<(>3!9e8[?!<_&e568c27ba+                                        "
#endif


TEST_CASE("Test utils::xorstr") {
    const std::string test_str = xorstr(ori_str);

    REQUIRE(ori_str == test_str);
}

#endif
