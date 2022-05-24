#pragma once

#include "KeyboardOperation.hpp"
#include "UIText.hpp"
#include "UnifiedFont.hpp"

AobaCreateEventComponentNS(InputField, ForbiddenCharInputted);
AobaCreateEventComponentNS(InputField, Inputted);
AobaCreateEventComponentNS(InputField, KeyEnterDown);

namespace s3d::aoba {
    class UIInputField : public UIText {
    public:
        enum class Type { Text, Number };

        enum class NumberType { Integer, Decimals };

        struct FormatResult {
            Optional<String> formattedText = none;
        };

        Type type             = Type::Text;
        NumberType numberType = NumberType::Decimals;
        size_t maxLength      = String::npos;
        double minNum = -DBL_MAX, maxNum = DBL_MAX;
        String prefix = U"", suffix = U"";
        Array<char32> forbiddenCharacters = {};

        // Includes \ / : * ? " < > |
        static const Array<char32> ForbiddenPathChar;

        // Includes ; , and space
        static const Array<char32> ForbiddenPathCharRecommended;

    private:
        KeyboardOperation<Input> operationCursorToLeft  = KeyboardOperation(KeyLeft, [this] { m_cursorPos--; });
        KeyboardOperation<Input> operationCursorToRight = KeyboardOperation(KeyRight, [this] { m_cursorPos++; });
        KeyboardOperation<InputGroup> operationSelectAll =
            KeyboardOperation(((KeyControl + KeyA) | (KeyCommand + KeyA)), [this] { selectAllText(); });
        KeyboardOperation<InputGroup> operationDeleteSelected =
            KeyboardOperation((KeyBackspace | KeyDelete), [this] { deleteSelectedText(); });
        KeyboardOperation<InputGroup> operationCopy =
            KeyboardOperation(((KeyControl + KeyC) | (KeyCommand + KeyC)), [this] { copyText(); });
        KeyboardOperation<InputGroup> operationPaste =
            KeyboardOperation(((KeyControl + KeyV) | (KeyCommand + KeyV)), [this] { pasteText(); });
        KeyboardOperation<InputGroup> operationCut =
            KeyboardOperation(((KeyControl + KeyX) | (KeyCommand + KeyX)), [this] {
                copyText();
                deleteSelectedText();
            });

        Optional<std::function<FormatResult(const String& str)>> m_textFormatter = none;

        bool m_textSelected           = false;
        bool m_selectingByKeyboard    = false;
        size_t m_selectingCursorStart = 0;

        bool m_isCursorVisible = true;
        RectF m_fieldRect, m_selectingRect;

        Stopwatch m_cursorBeamWatcher;
        size_t m_cursorPos      = 0;
        double m_cursorBeamPosX = 0;

        static UIText* ui_Warning;
        static size_t m_WarningTimeoutID;

    public:
        using UIText::UIText;

        virtual ~UIInputField() {}

        const RectF& fieldRect() const {
            return m_fieldRect;
        }

        void setText(const String& str) override;

        void setTextFormatter(const std::function<FormatResult(const String& str)>& formatter) {
            m_textFormatter = formatter;
        }

        void removeTextFormatter() {
            m_textFormatter = none;
        }

        double number() {
            if (text().length() == 1 && text().starts_with('-')) {
                return 0;
            }

            assert(type == Type::Number);
            return ParseFloat<double>(text());
        }

    protected:
        void initialize() override;

        void update() override;

        void updateInputEvents() override;

        void updateDrawableText(const Rect& scissor, bool updateField = false) override;

        void draw() const override;

        virtual String updateText();

        virtual FormatResult formatText(const String& str);

    private:
        void updateCursorMovement();

        void updateTextSelecting();

        void fireForbiddenCharsNotifier();

        void updateCursorBeamPos();

        void updateSelectingRect();

        void updateTextControls();

        FormatResult formatNumber(const String& str);

        // Controls
        void selectAllText();

        void deleteSelectedText();

        void copyText();

        void pasteText();
    };
}
