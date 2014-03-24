import QtQuick 1.0

Column {
    spacing: 10

    Row {
        spacing: 20
        PlanetLabel { }
        PlanetLabel { }
        PlanetLabel { }
        PlanetLabel { }
        PlanetLabel { }
    }

    Grid {
        spacing: 20
        rows: 2
        PlanetCard { }
        PlanetCard { }
        PlanetCard { }
        PlanetCard { }
        PlanetCard { }
        PlanetCard { }
        PlanetCard { }
        PlanetCard { }
        PlanetCard { }
        PlanetCard { }
    }

    Row {
        spacing: 20
        PlanetLabel { }
        PlanetLabel { }
        PlanetLabel { }
        PlanetLabel { }
        PlanetLabel { }
    }
}
