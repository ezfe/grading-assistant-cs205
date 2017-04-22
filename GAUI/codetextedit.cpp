#include "codetextedit.h"

/*!
 * @brief Sets up the code edit widget to be used to display code to be graded
 * @param parent - GradingSession
 */
CodeTextEdit::CodeTextEdit(QWidget *parent) : QPlainTextEdit(parent)
{
    lineNumberWidget = new LineNumberWidget(this);
    currentLineNumber = -1;

    //Connect signals/slots to update the edit depending on user actions
    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));
}


/*!
 * @brief Destructs everything created for this dialog
 */
CodeTextEdit::~CodeTextEdit()
{
    delete lineNumberWidget;
}


/*!
 * @brief Sets up the edit with the specified file and resets all associated
 * varaiables
 * @param filePath - path to current file
 */
void CodeTextEdit::setup_text(std::string filePath)
{
    //Reset edit and variables
    this->clear();
    extraSelections.clear();
    allSelections.clear();
    currentLineNumber = -1;

    //Open a file using the file path
    QFile file(QString::fromStdString(filePath));

    if(!file.open(QIODevice::ReadOnly)) {
        //error
    }

    //Put the contents of the file into the edit
    QTextStream in(&file);
    this->setPlainText(in.readAll());
    this->setReadOnly(true);
}


/*!
 * @brief Gets the currently selected line in the text edit
 * @return current line number
 */
int CodeTextEdit::get_current_line()
{
    return currentLineNumber;
}


/*!
 * @brief Highlight the line numbers given in the lines to highlight vector
 * @param linesToHighlight - line numbers to highlight
 */
void CodeTextEdit::setup_highlights(std::vector<int> linesToHighlight) {

    //get a copy of the current text cursor
    QTextCursor tempCursor = textCursor();

    //move the text cursor to the start of the document
    tempCursor.movePosition(QTextCursor::Start, QTextCursor::KeepAnchor, 1);


    //for each line number in the vector
    for(int i : linesToHighlight) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(Qt::blue).lighter(160);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);

        //calculate how many blocks away the cursor is from the line to highlight
        int blocksToMove = (i-1) - tempCursor.blockNumber();

        //move the cursor to the next block the calculated number of times
        tempCursor.movePosition(QTextCursor::NextBlock, QTextCursor::KeepAnchor, blocksToMove);

        //keep track of block number if need to remove later
        blockNumbers.push_back(tempCursor.blockNumber());

        selection.cursor = tempCursor;
        selection.cursor.clearSelection();

        //add this selection to list of all selections
        allSelections.append(selection);
    }

    //highlight these lines
    setExtraSelections(allSelections);
}


/*!
 * @brief Called when the user flags an annotation, highlights that line
 */
void CodeTextEdit::add_annotation()
{
    QColor lineColor = QColor(Qt::blue).lighter(160);

    //take the current selection (selection of current line), and turn the
    //highlight blue
    currentSelection.format.setBackground(lineColor);
    currentSelection.format.setProperty(QTextFormat::FullWidthSelection, true);

    //keep track of this block number if need to remove later
    blockNumbers.push_back(currentSelection.cursor.blockNumber());

    //add this to the list of all selections
    allSelections.append(currentSelection);

    //actually highlight
    setExtraSelections(allSelections);
}


/**
 * @brief Removes the selected annotation (GradingSession ensures that there is an
 * annotation there)
 */
void CodeTextEdit::remove_annotation()
{
    //find the index of this annotation in allSelections
    std::list<int>::iterator it = std::find(blockNumbers.begin(), blockNumbers.end(),
                                              textCursor().blockNumber());
    int index = distance(blockNumbers.begin(), it);

    //remove
    allSelections.removeAt(index);
    blockNumbers.remove(textCursor().blockNumber());

    //reset selections
    setExtraSelections(allSelections);
}


/*!
 * @brief Actually paints the line number area
 * @param event - paint event
 */
void CodeTextEdit::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberWidget);

    //paint background
    painter.fillRect(event->rect(), Qt::lightGray);

    //start with the first block
    QTextBlock block = firstVisibleBlock();

    //get the line number of that block
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();

    //while there are still blocks in the view
    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            //paint the current block number of the line number widget
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, lineNumberWidget->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        //move to next block
        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}


/*!
 * @brief Calculate the width of the line number area based on the number of
 * lines in the edit
 * @return amount of pixels the line number area takes up
 */
int CodeTextEdit::lineNumberAreaWidth()
{
    int digits = 1;

    //get the number of blocks, or lines, in the edit
    int max = qMax(1, blockCount());

    //count max number of digits in line number area
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    //calculate pixels based on width of digits
    int space = 3 + fontMetrics().width(QLatin1Char('9')) * digits;

    return space;
}


/*!
 * @brief Slot called when the widget is resized
 * @param e - resize event
 */
void CodeTextEdit::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    //update line number widget based on resize event
    QRect cr = contentsRect();
    lineNumberWidget->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}


/*!
 * @brief Called when the user changes the cursor postition, highlights the line
 * the user has clicked
 */
void CodeTextEdit::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    QTextEdit::ExtraSelection selection;

    QColor lineColor = QColor(Qt::yellow).lighter(160);

    selection.format.setBackground(lineColor);
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);
    selection.cursor = textCursor();
    selection.cursor.clearSelection();

    //update the current line numbe; current block number + 1
    currentLineNumber = selection.cursor.blockNumber() + 1;
    currentSelection = selection;

    //add this to extra selections, and then add the past highlights as well
    extraSelections.append(selection);
    extraSelections.append(allSelections);

    //actually highlight
    setExtraSelections(extraSelections);
}


/*!
 * @brief Updates the viewport based on the width of the line number area
 */
void CodeTextEdit::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}


/*!
 * @brief Update the line numbers seen in the edit based on the the user's
 * scrolling
 * @param rect - current rectangle view
 * @param dy - amount of vertical scroll
 */
void CodeTextEdit::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy) //if vertical scroll, scroll lineNumberWidet by same amount
        lineNumberWidget->scroll(0, dy);
    else //update based on x,y coordinate and width/height
        lineNumberWidget->update(0, rect.y(), lineNumberWidget->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}
