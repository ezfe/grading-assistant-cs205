#ifndef CODETEXTEDIT_H
#define CODETEXTEDIT_H

#include <QWidget>
#include <QPlainTextEdit>
#include <QPainter>
#include <QTextBlock>
#include <QFile>
#include <QTextStream>
#include <iostream>

class CodeTextEdit : public QPlainTextEdit
{
    Q_OBJECT

public:
    explicit CodeTextEdit(QWidget *parent = 0);
    //explicit CodeTextEdit(QWidget *parent = 0, std::string filePath = 0);
    ~CodeTextEdit();

    //Setup current text
    void setup_text(std::string filePath);

    int get_current_line();

    //Update/add highlights
    void setup_highlights(std::vector<int> toHighlight);

    void add_annotation();

    //Keep track/update line number area
    void lineNumberAreaPaintEvent(QPaintEvent *event);

    int lineNumberAreaWidth();

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void highlightCurrentLine();

    void updateLineNumberAreaWidth(int newCount);

    void updateLineNumberArea(const QRect &, int);

private:

    //Child widget allows the user to see line numbers
    QWidget *lineNumberWidget;

    //Keep track of selections (highlighted lines)
     QList<QTextEdit::ExtraSelection> extraSelections;
     QList<QTextEdit::ExtraSelection> allSelections;

     //Keep track of current line and selection
     int currentLineNumber;
     QTextEdit::ExtraSelection currentSelection;

};

/*!
 * @brief Represents the widget that keeps track of/paints the current line numbers.
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
