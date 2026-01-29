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
//<ID> 1379
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    char json_buf[] = "{ \"arr\": [ \"first\", \"second\", \"third\" ], \"meta\": \" some meta \" }";
    cJSON *arr = (cJSON *)0;
    cJSON *picked_item = (cJSON *)0;
    const char *picked_str = (const char *)0;
    cJSON *added_str = (cJSON *)0;
    int is_array_flag = 0;

    // step 2: Initialize - minify the JSON and parse into a cJSON structure
    cJSON_Minify(json_buf);
    root = cJSON_Parse(json_buf);

    // step 3: Configure - locate the array member within the parsed object
    arr = cJSON_GetObjectItem(root, "arr");

    // step 4: Operate - verify the node is an array, extract an element, and add it back as a named string
    is_array_flag = (int)cJSON_IsArray(arr);
    picked_item = cJSON_GetArrayItem(arr, 1);
    picked_str = cJSON_GetStringValue(picked_item);
    added_str = cJSON_AddStringToObject(root, "picked", picked_str);

    // step 5: Cleanup - free the constructed cJSON tree
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)is_array_flag;
    (void)picked_item;
    (void)picked_str;
    (void)added_str;
    return 66;
}