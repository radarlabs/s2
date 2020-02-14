{
  # NOTE: 'module_name' and 'module_path' come from the 'binary' property in package.json
  # node-pre-gyp handles passing them down to node-gyp when you build from source
  "targets": [{
    "target_name": "<(module_name)",

    "sources": [
      "./src/s2.cc",

      "./src/builder.cc",
      "./src/cell.cc",
      "./src/cell_id.cc",
      "./src/latlng.cc",
      "./src/loop.cc",
      "./src/point.cc",
      "./src/polygon.cc",
      "./src/region_coverer.cc",
      "./src/cell_union.cc",

      "./third_party/s2geometry/src/s2/base/stringprintf.cc",
      "./third_party/s2geometry/src/s2/third_party/absl/strings/ascii.cc",
      "./third_party/s2geometry/src/s2/util/coding/coder.cc",
      "./third_party/s2geometry/src/s2/util/math/exactfloat/exactfloat.cc",

      "./third_party/s2geometry/src/s2/encoded_s2cell_id_vector.cc",
      "./third_party/s2geometry/src/s2/encoded_s2point_vector.cc",
      "./third_party/s2geometry/src/s2/encoded_s2shape_index.cc",
      "./third_party/s2geometry/src/s2/encoded_string_vector.cc",
      "./third_party/s2geometry/src/s2/id_set_lexicon.cc",
      "./third_party/s2geometry/src/s2/mutable_s2shape_index.cc",
      "./third_party/s2geometry/src/s2/r2rect.cc",
      "./third_party/s2geometry/src/s2/s1angle.cc",
      "./third_party/s2geometry/src/s2/s1chord_angle.cc",
      "./third_party/s2geometry/src/s2/s1interval.cc",
      "./third_party/s2geometry/src/s2/s2boolean_operation.cc",
      "./third_party/s2geometry/src/s2/s2builder.cc",
      "./third_party/s2geometry/src/s2/s2builder_graph.cc",
      "./third_party/s2geometry/src/s2/s2builderutil_closed_set_normalizer.cc",
      "./third_party/s2geometry/src/s2/s2builderutil_find_polygon_degeneracies.cc",
      "./third_party/s2geometry/src/s2/s2builderutil_lax_polygon_layer.cc",
      "./third_party/s2geometry/src/s2/s2builderutil_s2point_vector_layer.cc",
      "./third_party/s2geometry/src/s2/s2builderutil_s2polygon_layer.cc",
      "./third_party/s2geometry/src/s2/s2builderutil_s2polyline_layer.cc",
      "./third_party/s2geometry/src/s2/s2builderutil_s2polyline_vector_layer.cc",
      "./third_party/s2geometry/src/s2/s2builderutil_snap_functions.cc",
      "./third_party/s2geometry/src/s2/s2cap.cc",
      "./third_party/s2geometry/src/s2/s2cell.cc",
      "./third_party/s2geometry/src/s2/s2cell_id.cc",
      "./third_party/s2geometry/src/s2/s2cell_index.cc",
      "./third_party/s2geometry/src/s2/s2cell_union.cc",
      "./third_party/s2geometry/src/s2/s2centroids.cc",
      "./third_party/s2geometry/src/s2/s2closest_cell_query.cc",
      "./third_party/s2geometry/src/s2/s2closest_edge_query.cc",
      "./third_party/s2geometry/src/s2/s2closest_point_query.cc",
      "./third_party/s2geometry/src/s2/s2contains_vertex_query.cc",
      "./third_party/s2geometry/src/s2/s2convex_hull_query.cc",
      "./third_party/s2geometry/src/s2/s2coords.cc",
      "./third_party/s2geometry/src/s2/s2crossing_edge_query.cc",
      "./third_party/s2geometry/src/s2/s2debug.cc",
      "./third_party/s2geometry/src/s2/s2earth.cc",
      "./third_party/s2geometry/src/s2/s2edge_clipping.cc",
      "./third_party/s2geometry/src/s2/s2edge_crosser.cc",
      "./third_party/s2geometry/src/s2/s2edge_crossings.cc",
      "./third_party/s2geometry/src/s2/s2edge_distances.cc",
      "./third_party/s2geometry/src/s2/s2edge_tessellator.cc",
      "./third_party/s2geometry/src/s2/s2error.cc",
      "./third_party/s2geometry/src/s2/s2furthest_edge_query.cc",
      "./third_party/s2geometry/src/s2/s2latlng.cc",
      "./third_party/s2geometry/src/s2/s2latlng_rect.cc",
      "./third_party/s2geometry/src/s2/s2latlng_rect_bounder.cc",
      "./third_party/s2geometry/src/s2/s2lax_loop_shape.cc",
      "./third_party/s2geometry/src/s2/s2lax_polygon_shape.cc",
      "./third_party/s2geometry/src/s2/s2lax_polyline_shape.cc",
      "./third_party/s2geometry/src/s2/s2loop.cc",
      "./third_party/s2geometry/src/s2/s2loop_measures.cc",
      "./third_party/s2geometry/src/s2/s2max_distance_targets.cc",
      "./third_party/s2geometry/src/s2/s2measures.cc",
      "./third_party/s2geometry/src/s2/s2metrics.cc",
      "./third_party/s2geometry/src/s2/s2min_distance_targets.cc",
      "./third_party/s2geometry/src/s2/s2padded_cell.cc",
      "./third_party/s2geometry/src/s2/s2point_compression.cc",
      "./third_party/s2geometry/src/s2/s2point_region.cc",
      "./third_party/s2geometry/src/s2/s2pointutil.cc",
      "./third_party/s2geometry/src/s2/s2polygon.cc",
      "./third_party/s2geometry/src/s2/s2polyline.cc",
      "./third_party/s2geometry/src/s2/s2polyline_alignment.cc",
      "./third_party/s2geometry/src/s2/s2polyline_measures.cc",
      "./third_party/s2geometry/src/s2/s2polyline_simplifier.cc",
      "./third_party/s2geometry/src/s2/s2predicates.cc",
      "./third_party/s2geometry/src/s2/s2projections.cc",
      "./third_party/s2geometry/src/s2/s2r2rect.cc",
      "./third_party/s2geometry/src/s2/s2region.cc",
      "./third_party/s2geometry/src/s2/s2region_coverer.cc",
      "./third_party/s2geometry/src/s2/s2region_intersection.cc",
      "./third_party/s2geometry/src/s2/s2region_term_indexer.cc",
      "./third_party/s2geometry/src/s2/s2region_union.cc",
      "./third_party/s2geometry/src/s2/s2shape_index.cc",
      "./third_party/s2geometry/src/s2/s2shape_index_buffered_region.cc",
      "./third_party/s2geometry/src/s2/s2shape_index_measures.cc",
      "./third_party/s2geometry/src/s2/s2shape_measures.cc",
      "./third_party/s2geometry/src/s2/s2shapeutil_build_polygon_boundaries.cc",
      "./third_party/s2geometry/src/s2/s2shapeutil_coding.cc",
      "./third_party/s2geometry/src/s2/s2shapeutil_contains_brute_force.cc",
      "./third_party/s2geometry/src/s2/s2shapeutil_edge_iterator.cc",
      "./third_party/s2geometry/src/s2/s2shapeutil_get_reference_point.cc",
      "./third_party/s2geometry/src/s2/s2shapeutil_range_iterator.cc",
      "./third_party/s2geometry/src/s2/s2shapeutil_visit_crossing_edge_pairs.cc",
      "./third_party/s2geometry/src/s2/s2text_format.cc",
      "./third_party/s2geometry/src/s2/s2wedge_relations.cc",
    ],
    "defines": [
      "NAPI_CPP_EXCEPTIONS",
      "NAPI_EXPERIMENTAL", # required for bigint
      "NDEBUG",
      "NODE_ADDON_API_DISABLE_DEPRECATED"
    ],
    "include_dirs": [
      "<!@(node -p \"require('node-addon-api').include\")",
      "./third_party/s2geometry/src"
    ],
    'conditions': [
      ['OS=="linux"', {
        'cflags!': [ '-fno-exceptions' ],
        'cflags_cc!': [ '-fno-exceptions' ],
        'cflags': [
           '-Wno-sign-compare',
           '-Wno-type-limits',
           '-Wno-comment',
           '-Wno-bool-compare',
           '-Wno-extra',
        ],
        "cflags+": [
          "-std=c++11"
        ],
        "cflags_c+": [
          "-std=c++14"
        ],
        "cflags_cc+": [
          "-std=c++14"
        ],
      }],
      ['OS=="mac"', {
        'xcode_settings': {
          'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
          'OTHER_CPLUSPLUSFLAGS':[
            '-Wno-unused-private-field',
            '-Wno-sign-compare',
            '-stdlib=libc++',
            '-std=c++14',
          ],
        },

      }]
    ]
  },
    {
      "target_name": "action_after_build",
      "type": "none",
      "dependencies": [ "<(module_name)" ],
      "copies": [
        {
          "files": [ "<(PRODUCT_DIR)/<(module_name).node" ],
          "destination": "<(module_path)"
        }
      ]
    }
]
}
