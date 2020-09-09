// /* When the user clicks on the button,
// toggle between hiding and showing the dropdown content */
var version = "v0.1";

var keywords = ["title", "options", "assign"];

var num_entries_per_category = {};

function createMain(index) {
    $.each(all_options, function (global_key, global_list) {
        // console.log(global_list[0]['options']);
        if(global_list[0]['options'] != undefined && global_list[0]['options'] == "hidden") return;

        var title = global_key;
        if(global_list[0]['title'] != undefined)
            title = global_list[0]['title'];
        // console.log(global_key, title);
        num_entries_per_category[global_key] = 0;
        var key_items = global_key.split(":");
        var name_key = key_items[0];
        var global_type = "global_type_";
        // if(key_items.length > 1) {
        if(global_list[0]['options'] != undefined) {
            // console.log(global_list[0]['options'].split(":")[0]);
            global_type += global_list[0]['options'].split(":")[0];
        }
        else {
            global_type += "none";
        }
        var list_div_id = name_key + "_div";
        $(".main_div").append('<div class="generated_div" id=\"' + list_div_id + '\"></div>');
        $("#" + list_div_id).prepend('<h2><a class="generated_div_title">' + title + '</a></h2>');
        $("#" + list_div_id).append('<ul class="generated_list ' + global_type + '" id=\"' + name_key + '_list\"></ul>');
        // console.log($("#" + list_div_id + " ul"));
        $("#" + list_div_id + " ul").sortable();
        createEntry(name_key, global_key);
        var params = '\'' + name_key + '\',\'' + global_key + '\'';
        $("#" + list_div_id).append('<div class="add_btn_div"><a class="icon_btn add_btn" onclick="createEntry(' + params + ')" ><i class="fa fa-plus"></i></a></div>');
        $(".main_div").append('<hr>');
    });
}

function createEntry(name_key, global_key)
{
    // console.log(name_key, global_key);
    num_entries_per_category[global_key] += 1;
    var index =  num_entries_per_category[global_key];
    var list_id = name_key + "_list";
    var list_name = "#" + list_id;

    var entry_id = 'entry_' + index;
    $(list_name).append('<li class="entry_group" id=' + entry_id + '></li>');
    $(list_name + " #" + entry_id).append('<div class="entry_box_div"><ul class="entry_box_list" id="options_list_' + index + '"></ul></div>');

    // console.log(global_key);
    if(global_key == "") return;
    $.each(all_options[global_key][0], function (key, list) {
        // console.log(key, $.inArray(key, keywords));
        if($.inArray(key, keywords) != -1) return;
        // console.log(key);
        $(list_name + " li #options_list_" + index).append('<li class="group_title" id=' + key.split(":")[0] + '></li>');
        var dropdown_list = $(list_name + " li").last();
        // console.log("Sending", list);
        createDropdownElement(dropdown_list, key, list);
    });

    var params = '\'' + name_key + '\', ' + index;
    $(list_name + " li #options_list_" + index).append('<li class="icon_btn_list_item"><a class="icon_btn remove_btn" onclick=\"removeMe(' + params + ')\"><i class="fa fa-times"></i></a></li>');

}

function removeMe(global_key,index)
{
    var list_id = global_key + "_list";
    var entry_element = "#" + list_id + " #entry_" + index;
    // console.log("Removing " + global_key + " " + index + " | " + entry_element);
    $(entry_element).remove();
    // num_entries_per_category[global_key] -= 1;
}

function entryClick(sender, text, is_array)
{
    // console.log(sender, text, is_array);
    if(is_array) {
        var element_rep = sender.closest('.entry_array_element');
        if(text == "REMOVE") {
            $(element_rep).remove();
        }
        else if ( element_rep.children[0].text != "+") {
            element_rep.children[0].text = text;
        }
        else {
            var element_clone = $(element_rep).clone();
            element_clone.prependTo(element_rep.closest(".entry_array"));
            element_rep = element_clone.children("a").first();
            element_rep.text(sender.text);
        }
    }
    else {
        sender.closest("div").children[1].text = text;
    }
}

function generateRecursiveListCall(source, key, layer) {
    var list = "";
    // console.log("Key", key);
    if(source == undefined) return list;
    $.each(source, function (sub_key, group) {
        if($.inArray(sub_key, keywords) != -1) return list;
        if($.type(group) == "string") {
            // console.log(">", key + "." + group, group);
            list += "," + key;
            if(group != "" ) list += "." + group;
        }
        else {
            var next_key = sub_key;
            if($.isNumeric(sub_key)) next_key = key;
            else if(key != "") next_key = key + "." + next_key;
            // console.log(">>", next_key);
            list += generateRecursiveListCall(group, next_key, layer + 1);
        }
    });
    return list;
}

function generateRecursiveList(source) {
    var recursive_list = "";
    string_list = generateRecursiveListCall(source, "", 0);
    return string_list.split(",");
}

function createDropdownElement(element, key, list) {
    // console.log(element, key, list);
    var btn_id = element.attr('id') + "_btn";
    element.append('<div class="option_list_div"></div');
    var option_list_div = element.children("div");

    var nest_count = 0;
    // console.log(list, list[0]['options'], list[0]['title'])
    if(list[0]['options'] != undefined && $.inArray(">", list[0]['options'].split(":")) >= 0) {
        var options = list[0]['options'].split(":");
        // console.log(options,$.inArray(">", options));
        nest_count = options[$.inArray(">", options)].match(/>/g).length;
    }
    // console.log(nest_count)
    key = key.split(":")[0];

    option_list_div.append('<p class="entry_list_title" id="' + nest_count + '">' + key + ':</p>');

    var list_head = list[0];
    if($.type(list_head) != "string" || list_head == "NONE") {
        list_head = list[1];
        // console.log("head", list_head, $.type(list_head));
    }
    var is_array = list_head.includes("array");
    // console.log(list, list_head, $.type(list_head), is_array);
    option_list_element = option_list_div
    if(is_array) {
        option_list_div.append('<ul class="entry_array"><li class="entry_array_element"></li</ul>');
        option_list_element = option_list_div.children("ul").children(0);
    }
    option_list_element.append('<a class="btn dropdown-toggle entry_text" id="' + btn_id + '" type="button" data-toggle="dropdown">+</a>');

    var tie_check = list_head.indexOf(":.");
    if(tie_check >= 0) {
        var valid_tie = false;
        var ii = 0;
        var tie_path = list_head.substring(tie_check+2, list_head.length).split(".");
        overcount = 3
        while(!valid_tie && ii < 2 && overcount-- > 0) {
            var sub_list = all_options;
            var last_path_key = "";
            console.log(overcount, valid_tie, ii, tie_path);
            var need_break = false;
            $.each(tie_path, function (i, path_key) {
                console.log( sub_list)
                if(need_break) return;

                if(path_key == "") {
                    sub_list = generateRecursiveList(sub_list);
                    list.splice(i-1,1);
                    sub_list.splice(0,1);
                    // console.log(list, list.splice(i-1,1))
                    // console.log(sub_list, sub_list.splice(0,1));
                    sub_list = $.merge(list,sub_list);
                    // console.log(list);

                    need_break = true;
                    valid_tie = true;
                }
                else {
                    // console.log("Path key", i, path_key);
                    if(sub_list[path_key] != undefined)
                    {
                        sub_list = sub_list[path_key];
                        if(i < tie_path.length - 1 && sub_list[0] != undefined) {
                            sub_list = sub_list[0];
                        }
                        if(sub_list[0] != undefined && $.type(sub_list[0]) == "string" && sub_list[0].includes(":.")) {

                            tie_check = sub_list[0].indexOf(":.");
                            // console.log("!!!", sub_list[0], tie_check);
                            tie_path = sub_list[0].substring(tie_check+2, sub_list[0].length).split(".");
                            ii++;
                            need_break = true;
                            valid_tie = false;
                        }
                        else {
                            last_path_key = path_key;
                            valid_tie = true;
                        }
                    }
                }
                // console.log("!!", valid_tie);
            });
            // console.log("!", valid_tie);
        }
        // console.log(tie_path);
        // console.log("List", sub_list);
        // console.log("List[0]", $.type(sub_list[0]));
        if($.type(sub_list) == "array" && $.type(sub_list[0]) == "object") {
            list = [];
            last_path_key = last_path_key.split(":")[0];
            // console.log(last_path_key, sub_list);
            $.each(sub_list[0], function (key, sub_list_item) {
                if(key == 'options') return;
                // console.log(last_path_key + "_" + key);
                list.push(last_path_key + "_" + key);
            });
        }
        else {
            list = sub_list;
        }
    }
    // console.log(list);
    createDropdown(option_list_element, key, list, btn_id, is_array);
}

function createDropdown(element, key, list, btn_id, is_array) {
    // console.log("createDropdown", element, key, list);
    var list_head =  list[0];
    element.append('<ul class="dropdown-menu" role="menu" aria-labelledby=\"' + btn_id + '\"></ul>');
    if(list_head == undefined) return;
    // console.log(">>", is_array, list_head, list);
    if(is_array && list_head != "REMOVE") {
        list.splice(0,0,"REMOVE");
    }
    $.each(list, function (i, item) {
        if(item == "array") return;
        // console.log(item);
        if($.type(item) != "string") return;
        // console.log("item", item);

        if(item.includes("enter")) {
            var prefix = "";
            if( list_head.includes("address") ) prefix = "0x";
            option_list_element.children(":last").append('<li class="option_dropdown" role="presentation">' + prefix + '<input class="text_entry_' + key + '" type="text" name="fname"></li>');
            $('.text_entry_' + key).bind("enterKey",function(e) { entryClick(e.target, prefix + e.target.value); });
            $('.text_entry_'+ key).keyup(function(e){ if(e.keyCode == 13) { $(this).trigger("enterKey"); } });
        }
        else {
            option_list_element.children(":last").append('<li class="option_dropdown" role="presentation"><a id="entry_option" role="menuitem" onclick="entryClick(this, this.text, ' + is_array + ')" tabindex="-1">' + item + '</a></li>');
        }
    });


}

$(document).ready(function() {
    console.log("loading");
    $( ".main_div" ).each(function( index ) {
        createMain(index);
    });

    $(".dropdown-toggle").dropdown();
});

function loadJSON() {
    $.each(profile, function(group_key, group) {
        // console.log(group_key, group);
        var found = false;
        var option_full_name = "";
        $.each(all_options, function(option_group_key, option_group) {
            if(found) return;
            if(option_group_key.substring(0, group_key.length) == group_key) {
                option_full_name = option_group_key;
                found = true;
            }
        });
        // console.log(group_key, option_full_name);

        var group_div = $("#" + group_key + "_div");
        if(group_div == undefined) return;

        var index = 0;
        $.each(group, function(item_key, item) {
            // console.log(item_key,item);
            if(item_key == "type") return;

            var item_index = parseInt(item_key.substring(group_key.length+1, item_key.length));
            // console.log(item_index + 1, group_div.children(".generated_list").children(".entry_group").length);
            if( item_index+1 > group_div.children(".generated_list").children(".entry_group").length ) {
                // console.log("Creating entry", group_key, option_full_name);
                createEntry(group_key, option_full_name);
            }

            var option_list = group_div.children(".generated_list").children(".entry_group").eq(index).children(".entry_box_div").children(".entry_box_list");
            $.each(item, function(item_list_key, item_list) {
                var to_enter = {};
                if($.isNumeric(item_list_key)) {
                    $.each(item_list, function(nested_item_list_key, nested_item_list) {
                        to_enter[nested_item_list_key] = nested_item_list;
                    });
                }
                else {
                    to_enter[item_list_key] = item_list;
                }
                $.each(to_enter, function(to_enter_key, to_enter_item) {
                    if(to_enter_key.substring(0,"num_".length) == "num_") return;
                    // console.log(item_list_key, item_list);
                    var item_list_element = option_list.children("#" + to_enter_key);
                    // console.log("#" + to_enter_key, item_list_element);
                    if(item_list_element == undefined) return;

                    // console.log($.type(item_list), item_list);
                    if($.type(to_enter_item) == "string") {
                        var options_source = item_list_element.children(".option_list_div").first().children(".dropdown-menu").children("li");
                        // console.log(options_source);
                        if(options_source.children("input").length > 0) {
                            var text_element = item_list_element.children(".option_list_div").first().children("a");
                            // console.log("text_element", text_element);
                            entryClick(options_source.children("input").first()[0], item_list, false);
                        }
                        else {
                            var found = false;
                            var options_to_trigger = options_source.children("a");
                            $.each(options_to_trigger, function(j, option) {
                                // console.log("option", to_enter_key, item_list, option.text);
                                if(to_enter_item == option.text) {
                                    $(option).trigger( "click" );
                                }
                            });
                        }
                    }
                    else {
                        var options_to_trigger = item_list_element.children(".option_list_div").children(".entry_array").children(".entry_array_element").first().children(".dropdown-menu").children("li").children("a");
                        $.each(to_enter_item, function(i, request) {
                            $.each(options_to_trigger, function(j, option) {
                                if(request == option.text) {
                                    // console.log(request, option);
                                    $(option).trigger( "click" );
                                }
                            });
                        });
                    }
                });
            });
            index++;
        });
    });
}

function generateJSON(print_json_string) {
    var full_parse = {};
    $.each($(".generated_list"), function (i, entry) {
        var entry_parse = {"type":"none"};
        var group_name = entry.id.replace("_list", "");
        var group_type_raw = $(entry).attr('class').split(' ')[1];
        // console.log(group_type_raw);
        var group_type = group_type_raw.substring("global_type_".length, group_type_raw.length);
        // console.log(group_type);
        entry_parse["type"] = group_type;
        $.each(entry.children, function (j, group) {
            if(group.className != 'entry_group') return;
            var component_name = group_name + "_" + j;
            entry_parse[component_name] = {};
            $.each(group.children[0].children[0].children, function (k, category) {
                // if(category.className != 'group_title') return;
                var data = category.children[0].children[1];
                var indent = category.children[0].children[0].id;
                if(entry_parse[component_name][indent] == undefined) {
                    if($.isNumeric(indent) && indent > 0) {
                        entry_parse[component_name][indent] = {};
                    }
                }
                if( data === undefined) return;
                else if ($(data).is("ul")) {
                    var name_list = {}; var num_elements = 0;
                    $.each(data.children, function(l, element_list) {
                        var name = element_list.children[0].text;
                        if(name == "+") return;
                        name_list[l] = name;
                        num_elements++;
                    });
                    if(num_elements == 0) name_list = "";

                    if(indent > 0) entry_parse[component_name][indent][category.id] = name_list;
                    else entry_parse[component_name][category.id] = name_list;

                    var num_id = category.id;
                    if(num_id[0] == ".") num_id = num_id.replace(".", ".num_");
                    else num_id = "num_" + num_id;
                    if(indent > 0) entry_parse[component_name][indent][num_id]= num_elements;
                    else entry_parse[component_name][num_id] = num_elements;
                }
                else {
                    var name = data.text;
                    if(category.id == name || name == "+") // check if unchanged
                    name = "";
                    if(indent > 0) entry_parse[component_name][indent][category.id] = name;
                    else entry_parse[component_name][category.id] = name;
                }
            });
        });
        full_parse[group_name] = entry_parse;
    });
    // console.log("print", print_json_string);
    if(print_json_string == true) {
        var json_txt = JSON.stringify(full_parse);
        console.log(json_txt);

        textPopupWithCopyClose("var profile = \n" + json_txt);
    }
    return full_parse;
}

function textPopupWithCopyClose(text) {
  $(".main_div").append('<textarea class="full_text" style="color:white">' + text + '</textarea>');
  $(".main_div").append('<p class="text_display">' + text.replace(/\n/g,"<br>") + '</p>');
  $(".text_display").on("click", function() {
      $(".full_text").select();
      console.log(document.execCommand("copy"));

      $(".full_text").remove();
      $(".text_display").remove();
  });
}

function today() {
    var d = new Date();
    return strDate = (d.getMonth()+1) + "/" + d.getDate() + "/" + d.getFullYear();
}

var profile_name = "C12_profile";

var c_header =
"\/\/ ********************** DO NOT EDIT - AUTO-GENERATED ********************** \/\/\n"
+ "\/\/ " + profile_name + ".h\n"
+ "\n"
+ "\/\/ Created by Combine Profile Generator " + version + " on " + today() + "\n"
+ "\/\/ Copyright \u00A9 Marbl. All rights reserved.\n"
+ "\n"
+ "#ifndef " + profile_name + "_h\n"
+ "#define " + profile_name + "_h\n"
+ "\n"
+ "#define NONE 0\n\n";

function generateC() {
    json = generateJSON(false);

    var full_text = c_header;
    full_text += generateOptions() + "\n";
    var collection = {};

    $.each(json, function (group_key, group) {
        var section_text = "";
        var include_group = false;
        var title = group_key;
        var option_group = "";
        var wrap = "";

        $.each(all_options, function(option_group_key, option_group_inner) {
            if(include_group) return;
            // console.log(group_key, option_group_key, option_group_inner[0]['options']);
            if(option_group_key == group_key
            && option_group_inner[0]['options'] != undefined
            && option_group_inner[0]['options'].includes('~')) {
                include_group = true;

                wrap = option_group_inner[0]['options'].split('~');
                wrap = wrap[wrap.length-1];
            }

            if(option_group_key == group_key
            && option_group_inner[0]['title'] != undefined) {
                title = option_group_inner[0]['title'];
            }
            // console.log(option_group_key, group_key, option_group_inner[0]['title'], title)
            if(option_group_key == group_key) {
                option_group = option_group_inner[0];
                // console.log(option_group);
            }
        });

        // console.log(group)
        // group_key = deplural(group_key);
        //
        // console.log(group_key, group);

        full_text += "\/\* " + title + " \*\/\n";
        switch (group["type"]) {
            case "list":
                $.each(group, function (entry_key, entry) {
                    if(!$.isPlainObject(entry)) return;
                    section_text = "";
                    var full_group_name = "";
                    var found_definition = false;
                    $.each(entry, function (list_key, list) {
                        if(!$.isPlainObject(list)) return;
                        found_definition = true;
                        full_group_name = group_key + "_" + list_key;
                        section_text += "#define " + full_group_name + " { ";

                        var group_prefix = "";
                        // console.log(list_key, option_group[list_key].length);
                        for(var j = 0; j < option_group[list_key].length; j++) {
                            if(option_group[list_key] != undefined
                                && option_group[list_key][j] != undefined
                                && option_group[list_key][j].split(':')[1] != undefined) {
                                var option_path = option_group[list_key][j].split(':')[1];
                                group_prefix = option_path.substring(1, option_path.length).replace('.',"_") + "_";
                                break;
                            }
                        }

                        $.each(list, function (i, list_item) {
                            // console.log(list_item);
                            section_text += group_prefix + list_item;
                            if(i < Object.keys(list).length - 1)
                            section_text += ", ";
                        });
                        section_text += " }\n";
                    });
                    if(!found_definition) return;
                    full_text += section_text;
                });
                full_text += "\n";
                break;
            case "group":
                if(include_group)
                collection[group_key] = [];
                $.each(group, function (entry_key, entry) {
                    section_text = "";
                    // console.log("entry", entry_key, entry);
                    var group_item_names = {};
                    if(!$.isPlainObject(entry)) return;
                    var group_name = entry_key;
                    var i = 0;
                    // console.log(entry_key);
                    $.each(entry, function (item_key, item) {
                        if($.isPlainObject(item)) {
                            console.log("Group should not have internal list.");
                        }
                        else {
                                // console.log(group_key, entry_key, entry, item_key, item);
                                var element_name = item.replace(item_key, '');
                                if(!item_key.toLowerCase().includes("pin") && !item_key.toLowerCase().includes("addr") ) {
                                    element_name = group_key + "_" + item_key + "_" + element_name;
                                }
                                if(item_key == "NAME") {
                                    group_name = item;
                                    element_name = '"' + element_name.split('_').slice(-1)[0] + '"';
                                }
                                // console.log(group_name, item_key, element_name);
                                section_text += "#define " + group_name + "_" + item_key + " " + element_name + "\n";
                                group_item_names[i++] = (group_name + "_" + item_key);
                            // }
                            // else {
                            //     group_name = item;
                            // }
                        }
                    });
                    if(group_name == "") return;
                    // console.log(group_name, group_key);
                    section_text += "#define " + group_name + "_" + group_key + " { ";
                    $.each(group_item_names, function (i, group_item) {
                        section_text += group_item;
                        if( i < Object.keys(group_item_names).length - 1)
                        section_text += ", ";
                    });
                    section_text += " }\n\n";
                    if(include_group)
                    collection[group_key].push(group_name + "_" + group_key);
                    full_text += section_text;
                });
                break;
            case "struct":
                // console.log("include", include_group, group_key)
                if(include_group) {
                    collection[group_key] = [];
                }
                $.each(group, function (entry_key, entry) {
                    section_text = "";
                    var id = "";
                    if(!$.isPlainObject(entry)) return;
                    $.each(entry, function (item_key, item) {
                        if($.isPlainObject(item)) {
                            section_text += "\t";
                            if(wrap == "") {
                                section_text += "." + item_key + " = "
                            }
                            section_text += "{ ";
                            var group_prefix = "";
                            // console.log(option_group, entry_key, entry, option_group[item_key], item_key);
                            // console.log()
                            for(var j = 0; j < option_group[item_key].length; j++) {
                                if(option_group[item_key] != undefined
                                    && option_group[item_key][j] != undefined
                                    && option_group[item_key][j].split(':')[1] != undefined) {
                                    var option_path = option_group[item_key][j].split(':')[1];
                                    group_prefix = option_path.substring(1, option_path.length).replace('.',"_") + "_";
                                    break;
                                }
                            }

                            $.each(item, function (i, list_item) {
                                // console.log(group_key, group, list_item);
                                if(list_item == "") section_text += "NONE";
                                else section_text += group_prefix + list_item;
                                if(i < Object.keys(item).length - 1)
                                section_text += ", ";
                            });
                            section_text += " }, \\\n";
                        }
                        else {
                            if(item_key == "ID") {
                                id = item;
                                var macro_name = item.replace("_ID", "");
                                // console.log(group_key, macro_name);
                                section_text += "#define " + group_key + "_" + macro_name + " ";
                                if(wrap != "") {
                                    section_text += group_key + "(";
                                }
                                else {
                                    section_text += "{";
                                }
                                section_text += " \\\n";
                                if(include_group) {
                                    collection[group_key].push(macro_name);
                                }
                            }
                            section_text += "\t";
                            if(wrap == "") {
                                section_text += "." + item_key + " = ";
                            }
                            if(item == "" ) section_text += "NONE";
                            else if(item_key.includes('num_')) {
                                section_text += item;
                            }
                            else {
                                // console.log(group_key, item_key, item);
                                if(item_key == item_key.toUpperCase()) section_text +=  group_key + "_" + item_key + "_";
                                section_text += item;
                            }

                            // console.log(item_key);
                            if(item_key != Object.keys(entry)[Object.keys(entry).length-1]) {
                                section_text += ",";
                            }
                            section_text += " \\\n";
                        }
                    });
                    if(id == "") return;
                    if(wrap != "") section_text += ")";
                    else section_text += "}";
                    section_text += "\n\n";
                    full_text += section_text;
                });
                break;
            case "nested":
                collection[group_key] = [];
                $.each(group, function (entry_key, entry) {
                    // console.log(":", entry);
                    if($.isPlainObject(entry)) {
                        section_text += "\t\t{ \\\n";
                        $.each(entry, function (list_key, list) {
                            if($.isPlainObject(list)) {
                                if($.isNumeric(Object.keys(list)[0])) {
                                    section_text += "\t\t\t{ \/* " + list_key + " *\/ \\\n";
                                    $.each(list, function (item_key, item) {
                                        // console.log(group_key, list_key, item_key, item);
                                        var group_prefix = "";
                                        for(var j = 0; j < option_group[list_key].length; j++) {
                                            if(option_group[list_key] != undefined
                                                && option_group[list_key][j] != undefined
                                                && $.type(option_group[list_key][j]) != "object"
                                                && option_group[list_key][j].split(':')[1] != undefined) {
                                                var option_path = option_group[list_key][j].split(':')[1];
                                                group_prefix = option_path.substring(1, option_path.length).replace('.',"_") + "_";
                                                // console.log(group_prefix);
                                                break;
                                            }
                                        }

                                        section_text += "\t\t\t\t" + group_prefix + item;
                                        // console.log(parseInt(item_key), Object.keys(list).length);
                                        if(parseInt(item_key) < Object.keys(list).length - 1) {
                                            section_text += ",";
                                        }
                                        section_text += " \\\n";
                                    });
                                    section_text += "\t\t\t}, \\\n";
                                }
                                else if(Object.keys(list).length > 0) {
                                    section_text += "\t\t\t{ \\\n";
                                    $.each(list, function (item_key, item) {
                                        if($.isPlainObject(item) || ( item == "" && $.type(item) == "string" )) {
                                            var group_prefix = "";
                                            for(var j = 0; j < option_group[item_key].length; j++) {
                                                if(option_group[item_key] != undefined
                                                    && option_group[item_key][j] != undefined
                                                    && $.type(option_group[item_key][j]) != "object"
                                                    && option_group[item_key][j].split(':')[1] != undefined) {
                                                    var option_path = option_group[item_key][j].split(':')[1];
                                                    group_prefix = option_path.substring(1, option_path.length).replace('.',"_") + "_";
                                                    // console.log(group_prefix);
                                                    break;
                                                }
                                            }

                                            section_text += "\t\t\t\t{ \/* " + item_key + " *\/ \\\n";

                                            $.each(item, function (nested_key, nested) {
                                                section_text += "\t\t\t\t\t" + group_prefix + nested;
                                                if(nested_key != Object.keys(item)[Object.keys(item).length-1])
                                                section_text += ",";
                                                section_text += " \\\n";
                                            });
                                            section_text += "\t\t\t\t}, \\\n";
                                        }
                                        else {
                                            var group_prefix = "";
                                            if($.isNumeric(item)) {
                                            }
                                            else if(option_group[item_key] != undefined) {
                                                for(var j = 0; j < option_group[item_key].length; j++) {
                                                    if(option_group[item_key] != undefined
                                                        && option_group[item_key][j] != undefined
                                                        && $.type(option_group[item_key][j]) != "object"
                                                        && option_group[item_key][j].split(':')[1] != undefined) {
                                                        var option_path = option_group[item_key][j].split(':')[1];
                                                        group_prefix = option_path.substring(1, option_path.length).replace('.',"_") + "_";
                                                        break;
                                                    }
                                                    else {
                                                        group_prefix = item_key + "_";
                                                    }
                                                }
                                            }
                                            comment = item_key;
                                            if( group_prefix != "" && !item_key.includes(group_key) ) {
                                                // console.log(group_prefix, group_key, item_key);
                                                group_prefix = group_key + "_" + group_prefix;
                                                comment = group_prefix.substring(0, group_prefix.length-1);
                                            }
                                            section_text += "\t\t\t\t" + group_prefix + item + ", \/* " + comment + " *\/ \\\n";
                                        }
                                    });
                                    section_text += "\t\t\t}, \\\n";
                                }
                            }
                            else {
                                // console.log(group_prefix, group_key, list_key, list);
                                var group_prefix = "";
                                var comment = list_key;
                                if(list_key == "NAME") {
                                    group_prefix = group_key + "_" + list_key + "_";
                                    comment = group_key + "_" + comment;
                                }
                                section_text += "\t\t\t" + group_prefix + list + ", \/* " + comment + " *\/ \\\n";
                            }
                        });
                        // section_text += ">\t\t\t},\n";
                        section_text += "\t\t}";
                        if(entry_key != Object.keys(group)[Object.keys(group).length-1])
                        section_text += ","
                        section_text += " \\\n";
                    }
                });
                // section_text += "\t\t}";
                // console.log(section_text);
                collection[group_key].push(section_text);
                break;
            default:
        }
        // if(section_text != "")
        // full_text += "\n";
    });

    section_text = "#define PROFILE_TEMPLATE \\\n{ \\\n"
    $.each(collection, function (group_key, group) {
        // console.log(group_key);
        section_text += "\t{ \/* " + group_key.split(":")[0] + " *\/ \\\n";
        if(!group[0].includes("{")) {
            section_text += "\t\t" + group.length + ", \\\n";
            section_text += "\t\t{ \\\n";
        }
        // section_text += "\t\t\t{\n";
        $.each(group, function(i, group_item) {
            // console.log("A", group_item);
            if(!group[0].includes("{")) {
                section_text += "\t\t\t";
            }
            // console.log(group_item, group_key);
            if(!group_item.includes("{")
                && !group_item.includes(group_key)) {
                section_text += group_key + "_" + group_item;
            }
            else {
                section_text += group_item;
            }

            if(!group[0].includes("{")) {
                if(i < group.length-1) section_text +=",";
                section_text += " \\\n";
            }
        });
        // section_text += "\t\t\t}\n";
        if(!group[0].includes("{")) {
            section_text += "\t\t} \\\n";
        }
        // }
        section_text += "\t}";
        if(group_key != Object.keys(collection)[Object.keys(collection).length-1]) {
            section_text += ", ";
        }
        section_text += " \\\n";
    });
    section_text += "};\n";
    section_text += "\n#endif \/*" + profile_name + "_h *\/";

    full_text += section_text;
    // console.log(full_text);
    textPopupWithCopyClose(full_text);
}

function generateOptions() {
    var full_text = "";
    $.each(all_options, function(global_key, global_item) {
        if(global_item[0]['options'] == undefined) return;
        var option_type = global_item[0]['options'].split(':')[0];
        if(option_type == 'list' || option_type == 'hidden') return;

        $.each(global_item[0], function(item_key, item_element) {
            // console.log(item_key, item_element);
            if($.inArray(item_key, keywords) != -1 || item_key == item_key.toLowerCase()) return;
            var section_text = "typedef enum\n{\n";

            var element_name = global_key + "_" + item_key;

            var first = true;
            // console.log(item_element);
            var assignments = [];
            var num = "";
            $.each(item_element, function(list_item_key, list_item) {
                if($.type(list_item) == "object" && list_item['assign'] != undefined) {
                    assignments = list_item['assign'].split(":");
                    return;
                }
                else if($.type(list_item) == "object" || list_item.includes(':.')) return;
                else if(list_item.includes('enter') || list_item.includes('array') || list_item.includes('REMOVE')) return;
                var list_element_name = "\t" + element_name + "_" + list_item.replace(item_key, '');
                // console.log(list_element_name);
                section_text += list_element_name;
                if(first)
                    section_text += " = 0";
                var offset = 1;
                if($.type(item_element[Object.keys(item_element).length-1]) == "object"
                && item_element[Object.keys(item_element).length-1]['assign'] != undefined) {
                    offset = 2;
                }
                if(list_item_key != Object.keys(item_element)[Object.keys(item_element).length-offset]) {
                    section_text += ",";
                }
                else {
                    num = "NUM_" + element_name;
                    section_text += ",\n\t" + num;
                }
                section_text += "\n";
                first = false;
            });

            section_text += "} " + element_name;

            // console.log(assignments);
            $.each(assignments, function(i, assignment) {
                if(i >= 1) {
                    if(assignment == assignment.toLowerCase()) {
                        assignment = assignments[0].toLowerCase() + "_" + assignment;
                    }
                    if(assignment == assignment.toUpperCase()) {
                        assignment = assignments[0].toUpperCase() + "_" + assignment;
                    }
                }
                section_text += ", " + assignment;
            });
            section_text += ";\n";

            if(num != "" && assignments.length > 0 && assignments[0] == assignments[0].toUpperCase()) {
                section_text += "#define " + "NUM_" + assignments[0] + " " + num + "\n";
            }

            if(!first)
                full_text += section_text + "\n";
        });

        // Debug loop
        $.each(global_item[0], function(item_key, item_element) {
            if($.type(item_element) == "string") return;
            // console.log(item_key, item_element);
            if(item_key == item_key.toLowerCase()) return;

            var assignments = [];
            var element_name = global_key + "_" + item_key;

            var section_text = "#ifdef DEBUG\n"
            + "static const char * " + element_name + "_STRINGS[] = \n{\n";

            var first = true;
            $.each(item_element, function(list_item_key, list_item) {
                if($.type(list_item) == "object" && list_item['assign'] != undefined) {
                    assignments = list_item['assign'].split(":");
                    return;
                }
                if($.type(list_item) == "object" || list_item.includes(':.')) return;
                else if(list_item.includes('enter') || list_item.includes('array') || list_item.includes('REMOVE')) return;
                var list_element_name = "\t\"" + element_name + "_" + list_item.replace(item_key, '') + "\"";
                // console.log(list_element_name);
                section_text += list_element_name;
                var offset = 1;
                if($.type(item_element[Object.keys(item_element).length-1]) == "object"
                && item_element[Object.keys(item_element).length-1]['assign'] != undefined) {
                    offset = 2;
                }
                if(list_item_key != Object.keys(item_element)[Object.keys(item_element).length-offset]) {
                    section_text += ",";
                }
                section_text += "\n";
                first = false;
            });

            section_text += "};\n";

            $.each(assignments, function(i, assignment) {
                // console.log(i, assignment);
                if(assignment == assignment.toLowerCase()) return;
                if(i >= 1) {
                    if(assignment == assignment.toUpperCase()) {
                       assignment = assignments[0].toUpperCase() + "_" + assignment;
                    }
                }
                section_text += "#define " + assignment + "_STRINGS " + element_name + "_STRINGS\n";
            });
            section_text += "#endif\n";

            if(!first)
                full_text += section_text + "\n";
        });
    });
    // console.log(full_text);
    return full_text;
}

function deplural(str) {
    // if(str[str.length-1] == "s" ) {
    //     if(str.substring(str.length-3, str.length-1) == "ie")
    //         return str.substring(0, str.length-3) + "y";
    //     else
    //         return str.substring(0, str.length-1);
    // }
    return str;
}
