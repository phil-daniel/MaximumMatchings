#include "Matching.h"

void Matching::augmentMatching(vector<vector<Edge>>* disjoint_augmenting_paths) {
    for (vector<Edge> augmenting_path : (*disjoint_augmenting_paths)) {
        for (Edge edge : augmenting_path) {
            // If the edge isn't in the matching, we add it to the matching.
            // Otherwise we remove it from the matching.
            if (matched_edges.find(edge) == matched_edges.end()) {
                matched_edges.insert(edge);
                vertex_to_matched_edge[edge.first] = edge;
                vertex_to_matched_edge[edge.second] = edge;
            } else {
                matched_edges.erase(edge);
                matched_edge_to_label.erase(edge);
                vertex_to_matched_edge.erase(edge.first);
                vertex_to_matched_edge.erase(edge.second);
            }
        }
    }
}

void Matching::addEdge(Edge edge) {
    matched_edges.insert(edge);
    vertex_to_matched_edge[edge.first] = edge;
    vertex_to_matched_edge[edge.second] = edge;
}

void Matching::removeEdge(Edge edge) {
    matched_edges.erase(edge);
    vertex_to_matched_edge.erase(edge.first);
    vertex_to_matched_edge.erase(edge.second);
}

bool Matching::isInMatching(Edge edge) {
    if (matched_edges.find(edge) == matched_edges.end()) {
        return false;
    }
    return true;
}

Edge Matching::getMatchedEdgeFromVertex(Vertex vertex) {
    if (vertex_to_matched_edge.find(vertex) == vertex_to_matched_edge.end()) {
        // If it's not involved in a matching return (-1, -1), i.e. an impossible edge.
        return make_pair(-1, -1);
    }
    return vertex_to_matched_edge[vertex];
}

int Matching::getLabel(Edge edge) {
    if (matched_edge_to_label.find(edge) == matched_edge_to_label.end()) {
        return -1;
    }
    return matched_edge_to_label[edge];
}

void Matching::setLabel(Edge edge, int label) {
    matched_edge_to_label[edge] = label;
}


void Matching::resetLabels() {
    // Emptying the map.
    matched_edge_to_label.clear();
    // Setting the current distance for each matched edge to infinity.
    for (Edge edge : matched_edges) {
        matched_edge_to_label[edge] = numeric_limits<int>::max();
    }
}

std::ostream &operator<<(std::ostream &os, Matching &matching) {
    os << "Matching:\n\t(Matched edge) : Label";
    for (Edge edge : matching.matched_edges) {
        os << "\n\t(" << edge.first << "->" << edge.second << ") : " << matching.getLabel(edge);
    }
    return os;
}