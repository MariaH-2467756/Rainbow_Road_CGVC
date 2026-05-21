#include "BezierCurve.h"
#include <vector>

// Fully rebuilt track with widened turns, relaxed spirals, and progressive
// transitions.
inline std::vector<BezierCurve> getCurves() {
  return {
      // 1-5: The Launch & Relaxed S-Curves (Gentler transitions, wider sweeps)
      BezierCurve({-20, 0, 0}, {-14, 0, 4}, {-8, 4, 8}, {-4, 6, 8}),
      BezierCurve({-4, 6, 8}, {0, 8, 8}, {4, 8, 4}, {8, 7, 0}),
      BezierCurve({8, 7, 0}, {12, 6, -4}, {16, 5, -8}, {22, 4, -8}),
      BezierCurve({22, 4, -8}, {28, 3, -8}, {32, 1, -4}, {34, 0, 0}),
      BezierCurve({34, 0, 0}, {35, -1, 3}, {32, -2, 8}, {26, -2, 12}),

      // 6-10: Widened Climbing Horseshoe (Expanded radius to prevent 180-degree
      // snapping)
      BezierCurve({26, -2, 12}, {20, -2, 16}, {14, 0, 22}, {8, 3, 24}),
      BezierCurve({8, 3, 24}, {2, 6, 26}, {-4, 9, 22}, {-4, 12, 16}),
      BezierCurve({-4, 12, 16}, {-4, 15, 10}, {2, 17, 4}, {8, 19, 4}),
      BezierCurve({8, 19, 4}, {14, 21, 4}, {22, 21, 8}, {26, 19, 14}),
      BezierCurve({26, 19, 14}, {30, 17, 20}, {32, 14, 26}, {26, 12, 32}),

      // 11-15: Smooth Drop into the Whirlpool Entry (Flattened the drop angle)
      BezierCurve({26, 12, 32}, {20, 10, 38}, {10, 6, 38}, {0, 3, 32}),
      BezierCurve({0, 3, 32}, {-10, 0, 26}, {-16, -1, 16}, {-16, -3, 6}),
      BezierCurve({-16, -3, 6}, {-16, -5, -4}, {-10, -7, -14}, {0, -9, -18}),
      BezierCurve({0, -9, -18}, {10, -11, -22}, {22, -11, -18}, {28, -11, -8}),
      BezierCurve({28, -11, -8}, {34, -11, 2}, {28, -13, 12}, {18, -15, 16}),

      // 16-25: THE WHIRLPOOL RE-ENGINEERED (Massively widened out, gradual
      // helix)
      // Dropped the tight inner layers entirely and replaced them with a
      // sweeping, predictable funnel.
      BezierCurve({18, -15, 16}, {8, -17, 20}, {-12, -19, 16}, {-20, -21, 4}),
      BezierCurve({-20, -21, 4}, {-28, -23, -8}, {-18, -25, -22},
                  {-4, -27, -24}),
      BezierCurve({-4, -27, -24}, {10, -29, -26}, {24, -31, -12}, {26, -33, 4}),
      BezierCurve({26, -33, 4}, {28, -35, 20}, {12, -38, 26}, {0, -40, 24}),
      BezierCurve({0, -40, 24}, {-12, -42, 22}, {-22, -44, 10}, {-22, -46, -4}),
      BezierCurve({-22, -46, -4}, {-22, -48, -18}, {-8, -50, -24},
                  {6, -52, -22}),
      BezierCurve({6, -52, -22}, {20, -54, -20}, {26, -56, -6}, {24, -58, 8}),
      BezierCurve({24, -58, 8}, {22, -60, 22}, {4, -62, 24}, {-10, -64, 18}),
      BezierCurve({-10, -64, 18}, {-24, -66, 12}, {-26, -68, -4},
                  {-18, -70, -16}),
      BezierCurve({-18, -70, -16}, {-10, -72, -28}, {6, -73, -28},
                  {16, -75, -16}),

      // 26-30: Smooth Slingshot Exit (Extended the escape ramp so it doesn't
      // break physics)
      BezierCurve({16, -75, -16}, {26, -77, -4}, {45, -50, 15}, {40, -20, 35}),
      BezierCurve({40, -20, 35}, {35, 10, 55}, {5, 30, 55}, {-15, 20, 35}),
      BezierCurve({-15, 20, 35}, {-35, 10, 15}, {-45, 0, -5}, {-35, -10, -25}),
      BezierCurve({-35, -10, -25}, {-25, -20, -45}, {5, -10, -55},
                  {25, 0, -45}),
      BezierCurve({25, 0, -45}, {45, 10, -35}, {55, 20, -15}, {50, 30, 5}),

      // 31-40: High Altitude Flattened Sweeps (Reduced deep zig-zag slicing)
      BezierCurve({50, 30, 5}, {45, 40, 25}, {25, 44, 35}, {5, 44, 25}),
      BezierCurve({5, 44, 25}, {-15, 44, 15}, {-35, 40, 5}, {-40, 35, -5}),
      BezierCurve({-40, 35, -5}, {-45, 30, -15}, {-35, 25, -35},
                  {-15, 20, -45}),
      BezierCurve({-15, 20, -45}, {5, 15, -55}, {25, 20, -65}, {45, 25, -60}),
      BezierCurve({45, 25, -60}, {65, 30, -55}, {75, 40, -35}, {70, 50, -15}),
      BezierCurve({70, 50, -15}, {65, 60, 5}, {45, 65, 25}, {25, 60, 35}),
      BezierCurve({25, 60, 35}, {5, 55, 45}, {-15, 50, 50}, {-35, 45, 45}),
      BezierCurve({-35, 45, 45}, {-55, 40, 40}, {-65, 30, 25}, {-60, 20, 5}),
      BezierCurve({-60, 20, 5}, {-55, 10, -15}, {-35, 5, -35}, {-15, 5, -45}),
      BezierCurve({-15, 5, -45}, {5, 5, -55}, {25, 10, -65}, {35, 15, -55}),

      // 41-49: The Winding Descent (Relaxed curves for gradual speed control)
      BezierCurve({35, 15, -55}, {45, 20, -45}, {50, 15, -25}, {45, 10, -5}),
      BezierCurve({45, 10, -5}, {40, 5, 15}, {25, 2, 25}, {5, 0, 30}),
      BezierCurve({5, 0, 30}, {-15, -2, 35}, {-35, 0, 25}, {-40, 5, 10}),
      BezierCurve({-40, 5, 10}, {-45, 10, -5}, {-40, 15, -20}, {-30, 15, -30}),
      BezierCurve({-30, 15, -30}, {-20, 15, -40}, {-5, 10, -45}, {5, 5, -40}),
      BezierCurve({5, 5, -40}, {15, 0, -35}, {25, -5, -25}, {30, -10, -10}),
      BezierCurve({30, -10, -10}, {35, -15, 5}, {25, -20, 20}, {10, -20, 25}),
      BezierCurve({10, -20, 25}, {-5, -20, 30}, {-20, -15, 25}, {-30, -10, 15}),
      BezierCurve({-30, -10, 15}, {-40, -5, 5}, {-40, 0, -5}, {-30, 0, -5}),

      // 50: Perfect Loop Connection (Blends seamlessly right back into Curve 1)
      BezierCurve({-30, 0, -5}, {-25, 0, -5}, {-26, 0, -4}, {-20, 0, 0})};
}