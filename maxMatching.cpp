#include <fstream>
#include <iostream>
#include <set>

// Required for hashing pairs
#include <boost/container_hash/hash.hpp>

#include "Stream/Stream.h"
#include "Stream/StreamFromFile.h"
#include "Stream/StreamFromMemory.h"

#include "Structures/FreeNodeStructure.h"

using namespace std;

typedef int Vertex;
typedef pair<Vertex, Vertex> Edge;
typedef set<Edge> Matching;
typedef unordered_map<Edge, int, boost::hash<Edge>> MatchingToLabel;

void extendActivePath(
    Stream* stream,
    Matching matching,
    float epsilon,
    vector<FreeNodeStructure> free_node_structs,
    vector<vector<Edge>> disjoint_augmenting_paths,
    set<Vertex> removed_vertices
) {

    unordered_map<Vertex, FreeNodeStructure*> vertex_to_free_node_struct;

    Edge edge = stream->readStream();
    // edges are only -1 if we have reached the end of the stream.
    while (edge.first != -1) {
        // Case 1 - If we have "removed" one of the vertices from the graph, we skip this edge.
        if (
            removed_vertices.find(edge.first) != removed_vertices.end() ||
            removed_vertices.find(edge.second) != removed_vertices.end()
        ) {
            edge = stream->readStream();
            continue;
        }

        // Case 2: If vertex1 is in the same blossom as vertex2, vertex1 isn't a working vertex
        // or the edge is already matched, we skip this edge.
        FreeNodeStructure* struct_of_u = nullptr;
        if (vertex_to_free_node_struct.count(edge.first) > 0) {
            struct_of_u = vertex_to_free_node_struct[edge.first];
        }
        FreeNodeStructure* struct_of_v = nullptr;
        if (vertex_to_free_node_struct.count(edge.first) > 0) {
            struct_of_v = vertex_to_free_node_struct[edge.first];
        }

        if (
            (
                struct_of_u != nullptr && struct_of_v != nullptr &&
                struct_of_u->getGraphNodeFromVertex(edge.first) == struct_of_v->getGraphNodeFromVertex(edge.second)
            ) ||
            (struct_of_u != nullptr && struct_of_u->working_vertex != edge.first) ||
            matching.find(edge) != matching.end()
        ) {
            edge = stream->readStream();
            continue;
        }

        // Case 3: If the first vertex is in a "marked" or "on hold" structure, we skip this edge.
        if (
            vertex_to_free_node_struct.count(edge.first) &&
            (
                vertex_to_free_node_struct[edge.first]->modified ||
                vertex_to_free_node_struct[edge.first]->on_hold
            )
        ) {
            edge = stream->readStream();
            continue;
        }

        // TODO: CASE 4 OUTER VERTEX
        if (true) {
            if (struct_of_u == struct_of_v) {
                // TODO: CONTRACT();
            } else {
                // TODO: AUGMENT();
            }
        }

        // TODO: CASE 5
        else {

        }


        // Reading next edge
        edge = stream->readStream();
    }

}


vector<vector<Edge>> algPhase(
    Stream* stream,
    Matching matching,
    float epsilon,
    float scale
) {
    vector<vector<Edge>> disjoint_augmenting_paths = {};

    set<Vertex> removed_vertices = {};

    MatchingToLabel matching_to_label;

    int path_limit = static_cast<int>(6 / scale) + 1;
    int pass_bundles_max = static_cast<int>(72 / (scale * epsilon));

    vector<FreeNodeStructure> free_node_structs;

    // Setting the current distance for each matched edge to infinity.
    for (Edge edge : matching) {
        matching_to_label[edge] = numeric_limits<int>::max();
    }

    for (int pass_bundle = 0; pass_bundle < pass_bundles_max; pass_bundle++) {
        for (FreeNodeStructure free_node_struct : free_node_structs) {
            if (free_node_struct.vertices_count >= path_limit) free_node_struct.on_hold = true;
            else free_node_struct.on_hold = false;
            free_node_struct.modified = false;
        }

        // TODO: IMPLEMENT ALGORITHM HERE!
        // EXTEND-ACTIVE-PATH()
        // CONTRACT-AND-AUGMENT()
        // BACKTRACK-STUCK-STRUCTURES()
    }


    return disjoint_augmenting_paths;
}

Matching augmentMatching(
    Matching matching,
    vector<vector<Edge>> disjoint_augmenting_paths
) {
    // Takes a vector (list) of disjoint augmenting paths and adds them to the matching.

    for (vector<Edge> augmenting_path : disjoint_augmenting_paths) {
        for (Edge edge : augmenting_path) {
            // If the edge isn't in the matching, we add it to the matching.
            // Otherwise we remove it from the matching.
            if (matching.find(edge) == matching.end()) {
                matching.insert(edge);
            } else {
                matching.erase(edge);
            }
        }
    }

    return matching;
}

Matching algorithm(
    Stream* stream,
    Matching matching,
    float epsilon
) {

    float scale_limit = (epsilon * epsilon) / 64;

    for (float scale = 1/2; scale <= scale_limit; scale *= 1/2) {
        float phase_limit = 144 / (scale * epsilon);

        for (float phase = 1; phase <= phase_limit; phase++) {
            vector<vector<Edge>> disjoint_augmenting_paths = {};
            matching = augmentMatching(matching, disjoint_augmenting_paths);
        }
    }

    return matching;
}

Matching get2ApproximateMatching(
    Stream* stream
) {
    std::set<int> involved_in_matching = set<int>();
    Matching matching;

    Edge edge = stream->readStream();
    // edges are only -1 if we have reached the end of the stream.
    while (edge.first != -1) {

        // 2-approximation within 1 pass, only adds to the matching if both vertices are currently not in the matching.
        if (
            involved_in_matching.find(edge.first) == involved_in_matching.end() &&
            involved_in_matching.find(edge.second) == involved_in_matching.end()
        ) {
            involved_in_matching.insert(edge.first);
            involved_in_matching.insert(edge.second);
            matching.insert(edge);
        }

        // Reading next edge
        edge = stream->readStream();
    }

    return matching;
}

int main() {

    //Stream* stream = new StreamFromFile("example.txt");
    Stream* stream = new StreamFromMemory("example.txt");

    Matching matching = get2ApproximateMatching(stream);

    std::cout << "Matching size: " << matching.size() << std::endl;

    return 0;
}
