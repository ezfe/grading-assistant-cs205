#include "codetextedit.h"

CodeTextEdit::CodeTextEdit(QWidget *parent, std::string filePath) :
    QPlainTextEdit(parent)
{
    lineNumberWidget = new LineNumberWidget(this);

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

    QFile file(QString::fromStdString(filePath));

    if(!file.open(QIODevice::ReadOnly)) {
        //error
    }

    QTextStream in(&file);
    this->setPlainText(in.readAll());
    this->setReadOnly(true);
}

CodeTextEdit::~CodeTextEdit()
{

}

int CodeTextEdit::get_current_line()
{
    return currentLineNumber;
}

int CodeTextEdit::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 3 + fontMetrics().width(QLatin1Char('9')) * digits;

    return space;
}

void CodeTextEdit::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void CodeTextEdit::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberWidget->scroll(0, dy);
    else
        lineNumberWidget->update(0, rect.y(), lineNumberWidget->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

void CodeTextEdit::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberWidget->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void CodeTextEdit::highlightCurrentLine()
{
    QTextEdit::ExtraSelection selection;

    QColor lineColor = QColor(Qt::yellow).lighter(160);

    selection.format.setBackground(lineColor);
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);
    selection.cursor = textCursor();
    selection.cursor.clearSelection();

    currentLineNumber = selection.cursor.blockNumber();

    int toDelete = -1;

    for(int i = 0; i < selectedLines.size(); i++) {
        //std::cerr << selectedLines[i] << std::endl;
        if(selectedLines[i] - 1 == currentLineNumber) {
            toDelete = i;
        }
    }

    if(toDelete >= 0)
    {
        extraSelections.removeAt(toDelete);
        selectedLines.erase(selectedLines.begin() + toDelete);

    }
    else {
        selectedLines.push_back(selection.cursor.blockNumber() + 1);
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

void CodeTextEdit::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberWidget);
    painter.fillRect(event->rect(), Qt::lightGray);


    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, lineNumberWidget->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}
