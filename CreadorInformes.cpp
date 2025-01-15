#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <hpdf.h>

struct Vehicle {
    std::string nom;
    std::string categoria;
    double preuAlquiler;
};

void generarPDF(const std::vector<Vehicle>& vehicles) {
    // Crear un nuevo documento PDF
    HPDF_Doc pdf = HPDF_New(nullptr, nullptr);
    if (!pdf) {
        std::cerr << "Error al crear el documento PDF.\n";
        return;
    }

    // Agregar una página
    HPDF_Page page = HPDF_AddPage(pdf);

    // Establecer el tamaño de la página
    HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);

    // Establecer las fuentes
    HPDF_Font font = HPDF_GetFont(pdf, "Helvetica", nullptr);
    HPDF_Page_SetFontAndSize(page, font, 12);

    // Establecer las coordenadas de inicio
    float y = 800;  // Altura de inicio de la página

    // Título
    HPDF_Page_BeginText(page);
    HPDF_Page_TextOut(page, 50, y, "Informe de inventario de vehiculos");
    HPDF_Page_EndText(page);

    // Fecha
    std::time_t ara = std::time(nullptr);
    struct tm tiempoLocal;
    localtime_s(&tiempoLocal, &ara);

    char fecha[100];
    std::strftime(fecha, sizeof(fecha), "%c", &tiempoLocal);
    HPDF_Page_BeginText(page);
    HPDF_Page_TextOut(page, 50, y - 20, "Fecha: ");
    HPDF_Page_TextOut(page, 100, y - 20, fecha);
    HPDF_Page_EndText(page);

    // Espacio para separar
    y -= 50;

    // Títulos de columna
    HPDF_Page_BeginText(page);
    HPDF_Page_TextOut(page, 50, y, "Nombre");
    HPDF_Page_TextOut(page, 200, y, "Categoria");
    HPDF_Page_TextOut(page, 350, y, "Precio Alquiler (€)");
    HPDF_Page_EndText(page);

    // Dibuja una línea de separación
    y -= 10;
    HPDF_Page_MoveTo(page, 50, y);
    HPDF_Page_LineTo(page, 500, y);
    HPDF_Page_Stroke(page);

    // Escribir los vehículos
    y -= 20;
    for (const auto& v : vehicles) {
        if (v.preuAlquiler > 50) {  // Filtrar vehículos con precio de alquiler mayor a 50
            HPDF_Page_BeginText(page);
            HPDF_Page_TextOut(page, 50, y, v.nom.c_str());
            HPDF_Page_TextOut(page, 200, y, v.categoria.c_str());
            HPDF_Page_TextOut(page, 350, y, std::to_string(v.preuAlquiler).c_str());
            HPDF_Page_EndText(page);
            y -= 20;  // Mover hacia abajo para el siguiente vehículo
        }
    }

    // Pie de página
    y -= 20;
    HPDF_Page_BeginText(page);
    HPDF_Page_TextOut(page, 50, y, "Informe generado automaticamente.");
    HPDF_Page_EndText(page);

    // Guardar el documento
    HPDF_SaveToFile(pdf, "informe_vehiculos.pdf");

    // Liberar recursos
    HPDF_Free(pdf);
}

int main() {
    std::vector<Vehicle> vehicles = {
        {"Toyota Corolla", "Sedan", 55.0},
        {"Ford Focus", "Hatchback", 45.0},
        {"BMW X5", "SUV", 120.0},
        {"Audi A3", "Sedan", 65.0},
        {"Mercedes GLA", "SUV", 95.0}
    };

    generarPDF(vehicles);

    std::cout << "Informe generado correctamente: informe_vehiculos.pdf\n";
    return 0;
}
