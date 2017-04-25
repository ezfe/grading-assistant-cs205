#ifndef CODETEXTEDIT_H
#define CODETEXTEDIT_H

#include <QWidget>
#include <QPlainTextEdit>
#include <QPainter>
#include <QTextBlock>
#include <QFile>
#include <QTextStream>
#include <iostream>

/*!
 * @brief The CodeTextEdit class creates the widget that allows the user to interact with a
 * student's code.
 *
 * This code is an extension of Qt's Code Editor example code, found on the Qt website at the
 * following link:
 *
 * http://doc.qt.io/qt-5/qtwidgets-widgets-codeeditor-example.html
 *
 * This code builds off Qt's provided design and methods to create the specific widget we needed
 * for our program.
 */
class CodeTextEdit : public QPlainTextEdit
{
    Q_OBJECT

public:

    //Constructor/Destructor
    explicit CodeTextEdit(QWidget *parent = 0);
    ~CodeTextEdit();

    //Setup current text
    void setup_text(std::string filePath);

    //Get Method
    int get_current_line();

    //Update/add highlights
    void setup_highlights(std::vector<int> toHighlight);

    void add_annotation();

    void remove_annotation();

    //Keep track/update line number area
    void lineNumberAreaPaintEvent(QPaintEvent *event);

    int lineNumberAreaWidth();

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:

    //Keep track of user actions/current view
    void highlightCurrentLine();

    void updateLineNumberAreaWidth(int newCount);

    void updateLineNumberArea(const QRect &, int);

private:

    //Child widget allows the user to see line numbers
    QWidget *lineNumberWidget;

    //Keep track of selections (highlighted lines)
    QList<QTextEdit::ExtraSelection> extraSelections;
    QList<QTextEdit::ExtraSelection> allSelections;
    std::list<int> blockNumbers;

    //Keep track of current line and selection
    int currentLineNumber;
    QTextEdit::ExtraSelection currentSelection;

};

/*!
 * @brief Represents the widget that keeps track of/paints the current line numbers.
 *
 * This code is an extension of Qt's Code Editor example code, found on the Qt website at the
 * following link:
 *
 * http://doc.qt.io/qt-5/qtwidgets-widgets-codeeditor-example.html
 */
class LineNumberWidget : public QWidget
{
public:
    LineNumberWidget(CodeTextEdit *editor) : QWidget(editor) {
        codeEditor = editor;
    }

    QSize sizeHint() const override {
        return QSize(codeEditor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        codeEditor->lineNumberAreaPaintEvent(event);
    }

private:

    //Parent widget
    CodeTextEdit *codeEditor;
};


#endif // CODETEXTEDIT_H
