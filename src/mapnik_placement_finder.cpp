/*****************************************************************************
 *
 * This file is part of Mapnik (c++ mapping toolkit)
 *
 * Copyright (C) 2024 Artem Pavlenko
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 *****************************************************************************/


#include <mapnik/config.hpp>
#include <mapnik/symbolizer.hpp>
#include <mapnik/symbolizer_hash.hpp>
#include <mapnik/symbolizer_utils.hpp>
#include <mapnik/symbolizer_keys.hpp>
#include <mapnik/symbolizer_enumerations.hpp>
#include <mapnik/text/placements/dummy.hpp>
#include <mapnik/text/placements/simple.hpp>
#include <mapnik/text/text_properties.hpp>
#include <mapnik/text/formatting/text.hpp>
#include "mapnik_symbolizer.hpp"
//pybind11
#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace
{

template <typename PlacementFinder>
void set_face_name(PlacementFinder & finder, std::string const& face_name)
{
    finder.defaults.format_defaults.face_name = face_name;
}

template <typename PlacementFinder>
std::string get_face_name(PlacementFinder const& finder)
{
    return finder.defaults.format_defaults.face_name;
}

template <typename PlacementFinder>
void set_text_size(PlacementFinder & finder, double text_size)
{
    finder.defaults.format_defaults.text_size = text_size;
}

template <typename PlacementFinder>
py::object get_text_size(PlacementFinder const& finder)
{
    return mapnik::util::apply_visitor(python_mapnik::extract_python_object<>(mapnik::keys::MAX_SYMBOLIZER_KEY),
                                       finder.defaults.format_defaults.text_size);
}

template <typename PlacementFinder>
void set_fill(PlacementFinder & finder, mapnik::color const& fill)
{
    finder.defaults.format_defaults.fill = fill;
}

template <typename PlacementFinder>
py::object get_fill(PlacementFinder const& finder)
{
    return mapnik::util::apply_visitor(python_mapnik::extract_python_object<>(mapnik::keys::MAX_SYMBOLIZER_KEY),
                                       finder.defaults.format_defaults.fill);
}

template <typename PlacementFinder>
void set_halo_fill(PlacementFinder & finder, mapnik::color const& halo_fill )
{
    finder.defaults.format_defaults.halo_fill = halo_fill;
}

template <typename PlacementFinder>
py::object get_halo_fill(PlacementFinder const& finder)
{
    return mapnik::util::apply_visitor(python_mapnik::extract_python_object<>(mapnik::keys::MAX_SYMBOLIZER_KEY),
                                       finder.defaults.format_defaults.halo_fill);
}

template <typename PlacementFinder>
void set_halo_radius(PlacementFinder & finder, double halo_radius)
{
    finder.defaults.format_defaults.halo_radius = halo_radius;
}

template <typename PlacementFinder>
py::object get_halo_radius(PlacementFinder const& finder)
{
    return mapnik::util::apply_visitor(python_mapnik::extract_python_object<>(mapnik::keys::MAX_SYMBOLIZER_KEY),
                                       finder.defaults.format_defaults.halo_radius);
}

template <typename PlacementFinder>
void set_format_expr(PlacementFinder & finder, std::string const& expr)
{
    finder.defaults.set_format_tree(
        std::make_shared<mapnik::formatting::text_node>(mapnik::parse_expression(expr)));
}

template <typename PlacementFinder>
std::string get_format_expr(PlacementFinder const& finder)
{
    mapnik::expression_set exprs;
    finder.defaults.add_expressions(exprs);
    std::string str = "";
    for (auto expr : exprs)
    {
        if (expr)
            str += mapnik::to_expression_string(*expr);
    }
    return str;
}

}

void export_placement_finder(py::module const& m)
{
    py::class_<mapnik::text_placements_dummy, std::shared_ptr<mapnik::text_placements_dummy>>(m, "PlacementFinder")
        .def(py::init<>(), "Default ctor")
        .def_property("face_name",
                      &get_face_name<mapnik::text_placements_dummy>,
                      &set_face_name<mapnik::text_placements_dummy>,
                      "Font face name")
        .def_property("text_size",
                      &get_text_size<mapnik::text_placements_dummy>,
                      &set_text_size<mapnik::text_placements_dummy>,
                      "Size of text")
        .def_property("fill",
                      &get_fill<mapnik::text_placements_dummy>,
                      &set_fill<mapnik::text_placements_dummy>,
                      "Fill")
        .def_property("halo_fill",
                      &get_halo_fill<mapnik::text_placements_dummy>,
                      &set_halo_fill<mapnik::text_placements_dummy>,
                      "Halo fill")
        .def_property("halo_radius",
                      &get_halo_radius<mapnik::text_placements_dummy>,
                      &set_halo_radius<mapnik::text_placements_dummy>,
                      "Halo radius")
        .def_property("format_expression",
                      &get_format_expr<mapnik::text_placements_dummy>,
                      &set_format_expr<mapnik::text_placements_dummy>,
                      "Format expression")
        ;

/*
    py::class_<mapnik::text_placements_simple, std::shared_ptr<mapnik::text_placements_simple>>(m, "PlacementFinderSimple")
        .def(py::init<>(), "Default ctor")
        .def_property("face_name",
                      &get_face_name<mapnik::text_placements_simple>,
                      &set_face_name<mapnik::text_placements_simple>,
                      "Font face name")
        .def_property("text_size",
                      &get_text_size<mapnik::text_placements_simple>,
                      &set_text_size<mapnik::text_placements_simple>,
                      "Size of text")
        .def_property("fill",
                      &get_fill<mapnik::text_placements_simple>,
                      &set_fill<mapnik::text_placements_simple>,
                      "Fill")
        .def_property("halo_fill",
                      &get_halo_fill<mapnik::text_placements_simple>,
                      &set_halo_fill<mapnik::text_placements_simple>,
                      "Halo fill")
        .def_property("halo_radius",
                      &get_halo_radius<mapnik::text_placements_simple>,
                      &set_halo_radius<mapnik::text_placements_simple>,
                      "Halo radius")
        .def_property("format_expression",
                      &get_format_expr<mapnik::text_placements_simple>,
                      &set_format_expr<mapnik::text_placements_simple>,
                      "Format expression")
        ;
*/
}
