/*==============================================================================

  Program: 3D Slicer

  Portions (c) Copyright Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

==============================================================================*/

///  vtkMarkupsGlyphSource2D - copies and extends vtkGlyphSource2D to
/// provide Slicer glyphs represented by poly data
///
/// vtkMarkupsGlyphSource2D can generate a family of 2D glyphs each of which lies
/// in the x-y plane (i.e., the z-coordinate is zero). The class is a helper
/// class to be used with vtkGlyph2D and vtkXYPlotActor.
//
/// To use this class, specify the glyph type to use and its
/// attributes. Attributes include its position (i.e., center point), scale,
/// color, and whether the symbol is filled or not (a polygon or closed line
/// sequence). You can also put a short line through the glyph running from -x
/// to +x (the glyph looks like it's on a line), or a cross.

#ifndef __vtkMarkupsGlyphSource2D_h
#define __vtkMarkupsGlyphSource2D_h

#include "vtkSlicerMarkupsModuleVTKWidgetsExport.h"

#include "vtkSlicerBaseLogic.h"

// VTK includes
#include "vtkGlyphSource2D.h"
#include "vtkPolyDataAlgorithm.h"

/// when add new glyph types, make sure to increase the clamp
#define VTK_STARBURST_GLYPH 12

class vtkPoints;
class vtkUnsignedCharArray;
class vtkCellArray;

class VTK_SLICER_MARKUPS_MODULE_VTKWIDGETS_EXPORT vtkMarkupsGlyphSource2D : public vtkPolyDataAlgorithm
{
public:
  vtkTypeMacro(vtkMarkupsGlyphSource2D,vtkPolyDataAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  /// Construct a vertex glyph centered at the origin, scale 1.0, white in
  /// color, filled, with line segment passing through the point.
  static vtkMarkupsGlyphSource2D *New();

  /// Set the center of the glyph. By default the center is (0,0,0).
  vtkSetVector3Macro(Center,double);
  vtkGetVectorMacro(Center,double,3);

  /// Set the scale of the glyph. Note that the glyphs are designed
  /// to fit in the (1,1) rectangle.
  vtkSetClampMacro(Scale,double,0.0,VTK_DOUBLE_MAX);
  vtkGetMacro(Scale,double);

  /// Set the scale of optional portions of the glyph (e.g., the
  /// dash and cross is DashOn() and CrossOn()).
  vtkSetClampMacro(Scale2,double,0.0,VTK_DOUBLE_MAX);
  vtkGetMacro(Scale2,double);

  /// Set the color of the glyph. The default color is white.
  vtkSetVector3Macro(Color,double);
  vtkGetVectorMacro(Color,double,3);

  /// Specify whether the glyph is filled (a polygon) or not (a
  /// closed polygon defined by line segments). This only applies
  /// to 2D closed glyphs.
  vtkSetMacro(Filled,int);
  vtkGetMacro(Filled,int);
  vtkBooleanMacro(Filled,int);

  /// Specify whether a short line segment is drawn through the
  /// glyph. (This is in addition to the glyph. If the glyph type
  /// is set to "Dash" there is no need to enable this flag.)
  vtkSetMacro(Dash,int);
  vtkGetMacro(Dash,int);
  vtkBooleanMacro(Dash,int);

  /// Specify whether a cross is drawn as part of the glyph. (This
  /// is in addition to the glyph. If the glyph type is set to
  /// "Cross" there is no need to enable this flag.)
  vtkSetMacro(Cross,int);
  vtkGetMacro(Cross,int);
  vtkBooleanMacro(Cross,int);

  /// Specify an angle (in degrees) to rotate the glyph around
  /// the z-axis. Using this ivar, it is possible to generate
  /// rotated glyphs (e.g., crosses, arrows, etc.)
  vtkSetMacro(RotationAngle,double);
  vtkGetMacro(RotationAngle,double);

  /// Specify the type of glyph to generate.
  vtkSetClampMacro(GlyphType,int,VTK_NO_GLYPH,VTK_STARBURST_GLYPH);
  vtkGetMacro(GlyphType,int);
  void SetGlyphTypeToNone() {this->SetGlyphType(VTK_NO_GLYPH);}
  void SetGlyphTypeToVertex() {this->SetGlyphType(VTK_VERTEX_GLYPH);}
  void SetGlyphTypeToDash() {this->SetGlyphType(VTK_DASH_GLYPH);}
  void SetGlyphTypeToCross() {this->SetGlyphType(VTK_CROSS_GLYPH);}
  void SetGlyphTypeToThickCross() {this->SetGlyphType(VTK_THICKCROSS_GLYPH);}
  void SetGlyphTypeToTriangle() {this->SetGlyphType(VTK_TRIANGLE_GLYPH);}
  void SetGlyphTypeToSquare() {this->SetGlyphType(VTK_SQUARE_GLYPH);}
  void SetGlyphTypeToCircle() {this->SetGlyphType(VTK_CIRCLE_GLYPH);}
  void SetGlyphTypeToDiamond() {this->SetGlyphType(VTK_DIAMOND_GLYPH);}
  void SetGlyphTypeToArrow() {this->SetGlyphType(VTK_ARROW_GLYPH);}
  void SetGlyphTypeToThickArrow() {this->SetGlyphType(VTK_THICKARROW_GLYPH);}
  void SetGlyphTypeToHookedArrow() {this->SetGlyphType(VTK_HOOKEDARROW_GLYPH);}
  void SetGlyphTypeToStarBurst() {this->SetGlyphType(VTK_STARBURST_GLYPH);}

  void SetGlyphTypeAsString(const char* type);

protected:
  vtkMarkupsGlyphSource2D();
  ~vtkMarkupsGlyphSource2D() {};

  int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *) override;

  double Center[3];
  double Scale;
  double Scale2;
  double Color[3];
  int   Filled;
  int   Dash;
  int   Cross;
  int   GlyphType;
  double RotationAngle;

  void TransformGlyph(vtkPoints *pts);
  void ConvertColor();
  unsigned char RGB[3];

  void CreateVertex(vtkPoints *pts, vtkCellArray *verts,
                    vtkUnsignedCharArray *colors);
  void CreateDash(vtkPoints *pts, vtkCellArray *lines,
                  vtkCellArray *polys, vtkUnsignedCharArray *colors, double scale = 1.0);
  void CreateCross(vtkPoints *pts, vtkCellArray *lines,
                   vtkCellArray *polys, vtkUnsignedCharArray *colors, double scale = 1.0);
  void CreateThickCross(vtkPoints *pts, vtkCellArray *lines,
                        vtkCellArray *polys, vtkUnsignedCharArray *colors);
  void CreateTriangle(vtkPoints *pts, vtkCellArray *lines,
                      vtkCellArray *polys, vtkUnsignedCharArray *colors);
  void CreateSquare(vtkPoints *pts, vtkCellArray *lines,
                    vtkCellArray *polys, vtkUnsignedCharArray *colors);
  void CreateCircle(vtkPoints *pts, vtkCellArray *lines,
                    vtkCellArray *polys, vtkUnsignedCharArray *colors);
  void CreateDiamond(vtkPoints *pts, vtkCellArray *lines,
                     vtkCellArray *polys, vtkUnsignedCharArray *colors);
  void CreateArrow(vtkPoints *pts, vtkCellArray *lines,
                   vtkCellArray *polys, vtkUnsignedCharArray *colors);
  void CreateThickArrow(vtkPoints *pts, vtkCellArray *lines,
                        vtkCellArray *polys, vtkUnsignedCharArray *colors);
  void CreateHookedArrow(vtkPoints *pts, vtkCellArray *lines,
                         vtkCellArray *polys, vtkUnsignedCharArray *colors);
  void CreateStarBurst(vtkPoints *pts, vtkCellArray *lines,
                       vtkCellArray *polys, vtkUnsignedCharArray *colors);

private:
  vtkMarkupsGlyphSource2D(const vtkMarkupsGlyphSource2D&);  /// Not implemented.
  void operator=(const vtkMarkupsGlyphSource2D&);  /// Not implemented.
};

#endif
