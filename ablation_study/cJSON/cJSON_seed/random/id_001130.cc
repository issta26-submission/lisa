#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 1130
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *name_item = (cJSON *)0;
    cJSON *age_item = (cJSON *)0;
    cJSON *colors = (cJSON *)0;
    cJSON *color0 = (cJSON *)0;
    cJSON *color1 = (cJSON *)0;
    cJSON *f_name = (cJSON *)0;
    cJSON *f_age = (cJSON *)0;
    cJSON *f_colors = (cJSON *)0;
    cJSON *f_color0 = (cJSON *)0;
    char *name_str = (char *)0;
    char *color0_str = (char *)0;
    double age_val = 0.0;
    cJSON_bool added_name = (cJSON_bool)0;
    cJSON_bool added_age = (cJSON_bool)0;
    cJSON_bool added_colors = (cJSON_bool)0;
    cJSON_bool added_c0 = (cJSON_bool)0;
    cJSON_bool added_c1 = (cJSON_bool)0;
    cJSON_bool is_age_num = (cJSON_bool)0;
    char *printed = (char *)0;
    int validation_score = 0;

    // step 2: Initialize - create root and items
    root = cJSON_CreateObject();
    name_item = cJSON_CreateString("Alice");
    age_item = cJSON_CreateNumber(30.0);
    colors = cJSON_CreateArray();
    color0 = cJSON_CreateString("red");
    color1 = cJSON_CreateString("blue");

    // step 3: Configure - attach items to root and populate array
    added_name = cJSON_AddItemToObject(root, "name", name_item);
    added_age = cJSON_AddItemToObject(root, "age", age_item);
    added_colors = cJSON_AddItemToObject(root, "colors", colors);
    added_c0 = cJSON_AddItemToArray(colors, color0);
    added_c1 = cJSON_AddItemToArray(colors, color1);

    // step 4: Operate - retrieve members and array elements
    f_name = cJSON_GetObjectItem(root, "name");
    name_str = cJSON_GetStringValue(f_name);
    f_age = cJSON_GetObjectItem(root, "age");
    age_val = cJSON_GetNumberValue(f_age);
    f_colors = cJSON_GetObjectItem(root, "colors");
    f_color0 = cJSON_GetArrayItem(f_colors, 0);
    color0_str = cJSON_GetStringValue(f_color0);

    // step 5: Validate - basic checks, serialize and free buffer
    is_age_num = cJSON_IsNumber(f_age);
    validation_score = (int)(root != (cJSON *)0) + (int)added_name + (int)added_age + (int)added_colors + (int)added_c0 + (int)added_c1 + (int)is_age_num + (int)(age_val == 30.0) + (int)(name_str != (char *)0 && name_str[0] == 'A') + (int)(color0_str != (char *)0 && color0_str[0] == 'r');
    printed = cJSON_PrintUnformatted(root);
    cJSON_free((void *)printed);
    (void)validation_score;

    // step 6: Cleanup - delete root which frees all attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}