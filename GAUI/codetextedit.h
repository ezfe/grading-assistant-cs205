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
    explicit CodeTextEdit(QWidget *parent = 0, std::string filePath = 0);
    ~CodeTextEdit();

    void lineNumberAreaPaintEvent(QPaintEvent *event);

    int lineNumberAreaWidth();

    int get_current_line();

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void highlightCurrentLine();

    void updateLineNumberAreaWidth(int newCount);

    void updateLineNumberArea(const QRect &, int);

private:

    QWidget *lineNumberWidget;

     QList<QTextEdit::ExtraSelection> extraSelections;

     std::vector<int> selectedLines;

     int currentLineNumber;

};


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
    CodeTextEdit *codeEditor;
};


#endif // CODETEXTEDIT_H
