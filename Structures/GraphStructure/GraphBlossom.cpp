#include "GraphBlossom.h"
#include "GraphVertex.h"

GraphBlossom::GraphBlossom() {
    isBlossom = true;
}

void GraphBlossom::addGraphNodeToBlossom(GraphNode* node) {
    nodesInBlossom.insert(node);
    for (GraphNode* child : node->children) {
        if (child->isBlossom) {
            GraphBlossom* blossom_child = dynamic_cast<GraphBlossom*>(child);
            child_to_blossom_vertex[child] = blossom_child->vertexToParent;
        } else {
            GraphVertex* vertex_child = dynamic_cast<GraphVertex*>(child);
            child_to_blossom_vertex[child] = vertex_child->vertex_id;
        }
    }
}

void GraphBlossom::printHelper(std::ostream &os, int depth) const {
    os << string(depth, '\t') << "Blossom:";
    for (GraphNode* node : nodesInBlossom) {
        if (node->isBlossom) {
            GraphBlossom* child_blossom = dynamic_cast<GraphBlossom*>(node);
            os << "\n";
            child_blossom->printHelper(os, depth+1);
        } else {
            GraphVertex* child_vertex = dynamic_cast<GraphVertex*>(node);
            os << "\n" << string(depth+1, '\t') << *child_vertex;
        }
    }
}

void GraphBlossom::print(std::ostream& os) const {
    printHelper(os, 0);
}

ostream &operator<<(std::ostream &os, const GraphBlossom &blossom) {
    blossom.print(os);
    return os;
}
