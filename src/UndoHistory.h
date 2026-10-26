#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Strokes.h"

struct Action {
    virtual ~Action() = default;

    virtual void Execute() = 0;
    virtual void Undo() = 0;
};

class UndoHistory {
    static std::vector<Action *> undoStack;
    static std::vector<Action *> redoStack;

  public:
    static void AddAction(Action *action);
    static void Undo();
    static void Redo();

    static const std::vector<Action *> &GetUndoStack() { return undoStack; }
    static const std::vector<Action *> &GetRedoStack() { return redoStack; }
};

struct AddStroke : public Action {
    Stroke *stroke;

    AddStroke(Stroke *stroke);
    void Execute() override;
    void Undo() override;
};
