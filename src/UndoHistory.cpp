#include "UndoHistory.h"

std::vector<Action *> UndoHistory::undoStack;
std::vector<Action *> UndoHistory::redoStack;

void UndoHistory::AddAction(Action *action) {
    action->Execute();
    undoStack.push_back(action);
    redoStack.clear();
}

void UndoHistory::Undo() {
    if (undoStack.size() == 0) return;
    undoStack.back()->Undo();
    redoStack.push_back(undoStack.back());
    undoStack.pop_back();
}

void UndoHistory::Redo() {
    if (redoStack.size() == 0) return;

    redoStack.back()->Execute();
    undoStack.push_back(redoStack.back());
    redoStack.pop_back();
}

AddStroke::AddStroke(Stroke *stroke) : stroke(stroke) {}
void AddStroke::Execute() { Strokes::AddStroke(stroke); }
void AddStroke::Undo() { Strokes::RemoveStroke(stroke); }
