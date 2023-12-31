#include "CHexagon.h"
CHexagon::CHexagon(int *_xCoords, int* _yCoords, int _vertexes, GfxInfo FigureGfxInfo) :CFigure(FigureGfxInfo)
{
    xCoordinates = _xCoords;
    yCoordinates = _yCoords;
	vertexes = _vertexes;
}


void CHexagon::DrawMe(GUI* pGUI) const
{
	//Call Output::DrawHexagon to draw a hexagon on the screen	
	pGUI->DrawHexagon(xCoordinates, yCoordinates, vertexes, FigGfxInfo, Selected);
}

bool CHexagon::IsPointInside(int x, int y)
{
    // Check if the point is within the bounding box of the hexagon
    int minX = *std::min_element(xCoordinates, xCoordinates + vertexes);
    int maxX = *std::max_element(xCoordinates, xCoordinates + vertexes);
    int minY = *std::min_element(yCoordinates, yCoordinates + vertexes);
    int maxY = *std::max_element(yCoordinates, yCoordinates + vertexes);

    if (x < minX || x > maxX || y < minY || y > maxY) {
        return false;  // The point is outside the bounding box
    }

    // Check if the point is inside the hexagon using a ray-casting algorithm
    int crossings = 0;
    for (int i = 0; i < vertexes; ++i) {
        int x1 = xCoordinates[i];
        int y1 = yCoordinates[i];
        int x2 = xCoordinates[(i + 1) % vertexes];
        int y2 = yCoordinates[(i + 1) % vertexes];

        if (((y1 <= y && y < y2) || (y2 <= y && y < y1)) &&
            x < (x2 - x1) * (y - y1) / (y2 - y1) + x1) {
            crossings++;
        }
    }

    // If the number of crossings is odd, the point is inside the hexagon
    return (crossings % 2 == 1);
}

string CHexagon::GetInfo()
{
    return "First point: (" + to_string(xCoordinates[0]) + ", " + to_string(yCoordinates[0]) + ")" +
        " - Second point: (" + to_string(xCoordinates[1]) + ", " + to_string(yCoordinates[1]) + ")"
        + " - vertexes count is: " + to_string(vertexes) + " - Area = " + to_string(CalcArea());
}

//omar
void CHexagon::resizeMe(int factor)
{
    // Calculate the center of the hexagon
    double centerX = 0, centerY = 0;
    for (int i = 0; i < vertexes; ++i) {
        centerX += xCoordinates[i];
        centerY += yCoordinates[i];
    }
    centerX /= vertexes;
    centerY /= vertexes;

    // Resize each vertex based on the factor
    for (int i = 0; i < vertexes; ++i) {
        double dx = xCoordinates[i] - centerX;
        double dy = yCoordinates[i] - centerY;

        // Resize the distance from the center by the factor
        xCoordinates[i] = centerX + factor * dx;
        yCoordinates[i] = centerY + factor * dy;
    }
}


float CHexagon::CalcArea()
{
    // Calculate the area of the hexagon by dividing it into six triangles
    float totalArea = 0.0;

    for (int i = 0; i < vertexes - 2; ++i) {
        // Get the coordinates of the current triangle's vertices
        float x1 = xCoordinates[0];
        float y1 = yCoordinates[0];
        float x2 = xCoordinates[i + 1];
        float y2 = yCoordinates[i + 1];
        float x3 = xCoordinates[i + 2];
        float y3 = yCoordinates[i + 2];

        // Calculate the area of the current triangle using the Shoelace formula
        float triangleArea = 0.5 * ((x1 * y2 + x2 * y3 + x3 * y1) - (x2 * y1 + x3 * y2 + x1 * y3));

        // Add the triangle's area to the total area
        totalArea += triangleArea;
    }

    return totalArea;
}