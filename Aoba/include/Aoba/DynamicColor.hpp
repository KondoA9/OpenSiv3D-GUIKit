#pragma once

#include "ColorTheme.hpp"

namespace s3d::aoba {
    /////////////////////////////////////////////////////
    // Material design colors
    // Light 0 ~ 9 Dark
    // From: https://materialui.co/colors
    // Official: https://material.io/design/color/the-color-system.html
    /////////////////////////////////////////////////////
    namespace MaterialColor {
        constexpr auto Red0 = Color(255, 235, 238);
        constexpr auto Red1 = Color(255, 205, 210);
        constexpr auto Red2 = Color(239, 154, 154);
        constexpr auto Red3 = Color(229, 115, 115);
        constexpr auto Red4 = Color(239, 83, 80);
        constexpr auto Red5 = Color(244, 67, 54);
        constexpr auto Red6 = Color(229, 57, 53);
        constexpr auto Red7 = Color(211, 47, 47);
        constexpr auto Red8 = Color(198, 40, 40);
        constexpr auto Red9 = Color(183, 28, 28);

        constexpr auto Pink0 = Color(252, 228, 236);
        constexpr auto Pink1 = Color(248, 187, 208);
        constexpr auto Pink2 = Color(244, 143, 177);
        constexpr auto Pink3 = Color(240, 98, 146);
        constexpr auto Pink4 = Color(236, 64, 122);
        constexpr auto Pink5 = Color(233, 30, 99);
        constexpr auto Pink6 = Color(216, 27, 96);
        constexpr auto Pink7 = Color(194, 24, 91);
        constexpr auto Pink8 = Color(173, 20, 87);
        constexpr auto Pink9 = Color(136, 14, 79);

        constexpr auto Purple0 = Color(243, 229, 245);
        constexpr auto Purple1 = Color(225, 190, 231);
        constexpr auto Purple2 = Color(206, 147, 216);
        constexpr auto Purple3 = Color(186, 104, 200);
        constexpr auto Purple4 = Color(171, 71, 188);
        constexpr auto Purple5 = Color(156, 39, 176);
        constexpr auto Purple6 = Color(142, 36, 170);
        constexpr auto Purple7 = Color(123, 31, 162);
        constexpr auto Purple8 = Color(106, 27, 154);
        constexpr auto Purple9 = Color(74, 20, 140);

        constexpr auto DeepPurple0 = Color(237, 231, 246);
        constexpr auto DeepPurple1 = Color(209, 196, 233);
        constexpr auto DeepPurple2 = Color(179, 157, 219);
        constexpr auto DeepPurple3 = Color(149, 117, 205);
        constexpr auto DeepPurple4 = Color(126, 87, 194);
        constexpr auto DeepPurple5 = Color(103, 58, 183);
        constexpr auto DeepPurple6 = Color(94, 53, 177);
        constexpr auto DeepPurple7 = Color(81, 45, 168);
        constexpr auto DeepPurple8 = Color(69, 39, 160);
        constexpr auto DeepPurple9 = Color(49, 27, 146);

        constexpr auto Indigo0 = Color(232, 234, 246);
        constexpr auto Indigo1 = Color(197, 202, 233);
        constexpr auto Indigo2 = Color(159, 168, 218);
        constexpr auto Indigo3 = Color(121, 134, 203);
        constexpr auto Indigo4 = Color(92, 107, 192);
        constexpr auto Indigo5 = Color(63, 81, 181);
        constexpr auto Indigo6 = Color(57, 73, 171);
        constexpr auto Indigo7 = Color(48, 63, 159);
        constexpr auto Indigo8 = Color(40, 53, 147);
        constexpr auto Indigo9 = Color(26, 35, 126);

        constexpr auto Blue0 = Color(227, 242, 253);
        constexpr auto Blue1 = Color(187, 222, 251);
        constexpr auto Blue2 = Color(144, 202, 249);
        constexpr auto Blue3 = Color(100, 181, 246);
        constexpr auto Blue4 = Color(66, 165, 245);
        constexpr auto Blue5 = Color(33, 150, 243);
        constexpr auto Blue6 = Color(30, 136, 229);
        constexpr auto Blue7 = Color(25, 118, 210);
        constexpr auto Blue8 = Color(21, 101, 192);
        constexpr auto Blue9 = Color(13, 71, 161);

        constexpr auto LightBlue0 = Color(225, 245, 254);
        constexpr auto LightBlue1 = Color(179, 229, 252);
        constexpr auto LightBlue2 = Color(129, 212, 250);
        constexpr auto LightBlue3 = Color(79, 195, 247);
        constexpr auto LightBlue4 = Color(41, 182, 246);
        constexpr auto LightBlue5 = Color(3, 169, 244);
        constexpr auto LightBlue6 = Color(3, 155, 229);
        constexpr auto LightBlue7 = Color(2, 136, 209);
        constexpr auto LightBlue8 = Color(2, 119, 189);
        constexpr auto LightBlue9 = Color(1, 87, 155);

        constexpr auto Cyan0 = Color(224, 247, 250);
        constexpr auto Cyan1 = Color(178, 235, 242);
        constexpr auto Cyan2 = Color(128, 222, 234);
        constexpr auto Cyan3 = Color(77, 208, 225);
        constexpr auto Cyan4 = Color(38, 198, 218);
        constexpr auto Cyan5 = Color(0, 188, 212);
        constexpr auto Cyan6 = Color(0, 172, 193);
        constexpr auto Cyan7 = Color(0, 151, 167);
        constexpr auto Cyan8 = Color(2, 119, 189);
        constexpr auto Cyan9 = Color(0, 96, 100);

        constexpr auto Teal0 = Color(224, 242, 241);
        constexpr auto Teal1 = Color(178, 223, 219);
        constexpr auto Teal2 = Color(128, 203, 196);
        constexpr auto Teal3 = Color(77, 182, 172);
        constexpr auto Teal4 = Color(38, 166, 154);
        constexpr auto Teal5 = Color(0, 150, 136);
        constexpr auto Teal6 = Color(0, 137, 123);
        constexpr auto Teal7 = Color(0, 121, 107);
        constexpr auto Teal8 = Color(0, 105, 92);
        constexpr auto Teal9 = Color(0, 77, 64);

        constexpr auto Green0 = Color(232, 245, 233);
        constexpr auto Green1 = Color(200, 230, 201);
        constexpr auto Green2 = Color(165, 214, 167);
        constexpr auto Green3 = Color(129, 199, 132);
        constexpr auto Green4 = Color(102, 187, 106);
        constexpr auto Green5 = Color(76, 175, 80);
        constexpr auto Green6 = Color(67, 160, 71);
        constexpr auto Green7 = Color(56, 142, 60);
        constexpr auto Green8 = Color(46, 125, 50);
        constexpr auto Green9 = Color(27, 94, 32);

        constexpr auto LightGreen0 = Color(241, 248, 233);
        constexpr auto LightGreen1 = Color(220, 237, 200);
        constexpr auto LightGreen2 = Color(197, 225, 165);
        constexpr auto LightGreen3 = Color(174, 213, 129);
        constexpr auto LightGreen4 = Color(156, 204, 101);
        constexpr auto LightGreen5 = Color(139, 195, 74);
        constexpr auto LightGreen6 = Color(124, 179, 66);
        constexpr auto LightGreen7 = Color(104, 159, 56);
        constexpr auto LightGreen8 = Color(85, 139, 47);
        constexpr auto LightGreen9 = Color(27, 94, 32);

        constexpr auto Lime0 = Color(249, 251, 231);
        constexpr auto Lime1 = Color(240, 244, 195);
        constexpr auto Lime2 = Color(230, 238, 156);
        constexpr auto Lime3 = Color(220, 231, 117);
        constexpr auto Lime4 = Color(212, 225, 87);
        constexpr auto Lime5 = Color(205, 220, 57);
        constexpr auto Lime6 = Color(192, 202, 51);
        constexpr auto Lime7 = Color(175, 180, 43);
        constexpr auto Lime8 = Color(158, 157, 36);
        constexpr auto Lime9 = Color(130, 119, 23);

        constexpr auto Yellow0 = Color(255, 253, 231);
        constexpr auto Yellow1 = Color(255, 249, 196);
        constexpr auto Yellow2 = Color(255, 245, 157);
        constexpr auto Yellow3 = Color(255, 241, 118);
        constexpr auto Yellow4 = Color(255, 238, 88);
        constexpr auto Yellow5 = Color(255, 235, 59);
        constexpr auto Yellow6 = Color(253, 216, 53);
        constexpr auto Yellow7 = Color(251, 192, 45);
        constexpr auto Yellow8 = Color(249, 168, 37);
        constexpr auto Yellow9 = Color(245, 127, 23);

        constexpr auto Amber0 = Color(255, 248, 225);
        constexpr auto Amber1 = Color(255, 236, 179);
        constexpr auto Amber2 = Color(255, 224, 130);
        constexpr auto Amber3 = Color(255, 213, 79);
        constexpr auto Amber4 = Color(255, 202, 40);
        constexpr auto Amber5 = Color(255, 193, 7);
        constexpr auto Amber6 = Color(255, 179, 0);
        constexpr auto Amber7 = Color(255, 160, 0);
        constexpr auto Amber8 = Color(249, 168, 37);
        constexpr auto Amber9 = Color(245, 127, 23);

        constexpr auto Orange0 = Color(255, 243, 224);
        constexpr auto Orange1 = Color(255, 224, 178);
        constexpr auto Orange2 = Color(255, 204, 128);
        constexpr auto Orange3 = Color(255, 183, 77);
        constexpr auto Orange4 = Color(255, 167, 38);
        constexpr auto Orange5 = Color(255, 152, 0);
        constexpr auto Orange6 = Color(251, 140, 0);
        constexpr auto Orange7 = Color(245, 124, 0);
        constexpr auto Orange8 = Color(239, 108, 0);
        constexpr auto Orange9 = Color(230, 81, 0);

        constexpr auto DeepOrange0 = Color(251, 233, 231);
        constexpr auto DeepOrange1 = Color(255, 204, 188);
        constexpr auto DeepOrange2 = Color(255, 171, 145);
        constexpr auto DeepOrange3 = Color(255, 138, 101);
        constexpr auto DeepOrange4 = Color(255, 112, 67);
        constexpr auto DeepOrange5 = Color(255, 87, 34);
        constexpr auto DeepOrange6 = Color(244, 81, 30);
        constexpr auto DeepOrange7 = Color(230, 74, 25);
        constexpr auto DeepOrange8 = Color(216, 67, 21);
        constexpr auto DeepOrange9 = Color(191, 54, 12);

        constexpr auto Brown0 = Color(239, 235, 233);
        constexpr auto Brown1 = Color(215, 204, 200);
        constexpr auto Brown2 = Color(188, 170, 164);
        constexpr auto Brown3 = Color(161, 136, 127);
        constexpr auto Brown4 = Color(141, 110, 99);
        constexpr auto Brown5 = Color(121, 85, 72);
        constexpr auto Brown6 = Color(109, 76, 65);
        constexpr auto Brown7 = Color(93, 64, 55);
        constexpr auto Brown8 = Color(78, 52, 46);
        constexpr auto Brown9 = Color(62, 39, 35);

        constexpr auto Grey0 = Color(250, 250, 250);
        constexpr auto Grey1 = Color(245, 245, 245);
        constexpr auto Grey2 = Color(238, 238, 238);
        constexpr auto Grey3 = Color(224, 224, 224);
        constexpr auto Grey4 = Color(189, 189, 189);
        constexpr auto Grey5 = Color(158, 158, 158);
        constexpr auto Grey6 = Color(117, 117, 117);
        constexpr auto Grey7 = Color(97, 97, 97);
        constexpr auto Grey8 = Color(66, 66, 66);
        constexpr auto Grey9 = Color(33, 33, 33);

        constexpr auto BlueGrey0 = Color(236, 239, 241);
        constexpr auto BlueGrey1 = Color(207, 216, 220);
        constexpr auto BlueGrey2 = Color(176, 190, 197);
        constexpr auto BlueGrey3 = Color(144, 164, 174);
        constexpr auto BlueGrey4 = Color(120, 144, 156);
        constexpr auto BlueGrey5 = Color(96, 125, 139);
        constexpr auto BlueGrey6 = Color(84, 110, 122);
        constexpr auto BlueGrey7 = Color(69, 90, 100);
        constexpr auto BlueGrey8 = Color(55, 71, 79);
        constexpr auto BlueGrey9 = Color(38, 50, 56);
    }

    namespace DynamicColor {
        constexpr auto Clear = ColorTheme(Color(0, 0, 0, 0));

        constexpr auto Background          = ColorTheme(MaterialColor::Grey0, MaterialColor::Grey9);
        constexpr auto BackgroundSecondary = ColorTheme(MaterialColor::BlueGrey0, MaterialColor::Grey8);
        constexpr auto BackgroundTertiary  = ColorTheme(MaterialColor::BlueGrey1, MaterialColor::Grey7);

        constexpr auto BackgroundHovered = ColorTheme(MaterialColor::Grey1, MaterialColor::Grey8);

        constexpr auto Separator = ColorTheme(MaterialColor::Grey4, MaterialColor::Grey7);

        constexpr auto Text = ColorTheme(MaterialColor::Grey9, MaterialColor::Grey0);
    }
}
