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
//<ID> 685
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char json_buf[] = "  { \"initial\": 1, \"list\": [ \"x\", \"y\" ] }  ";
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *new_str = (cJSON *)0;
    char *printed = (char *)0;
    int arr_size = 0;
    cJSON_bool added_single = 0;

    // step 2: Initialize
    cJSON_Minify(json_buf);
    root = cJSON_Parse(json_buf);

    // step 3: Configure
    arr = cJSON_AddArrayToObject(root, "appended");
    new_str = cJSON_CreateString("inserted");
    added_single = cJSON_AddItemToObject(root, "single", new_str);

    // step 4: Operate
    printed = cJSON_PrintUnformatted(root);
    arr_size = cJSON_GetArraySize(arr);

    // step 5: Validate
    (void)added_single;
    (void)arr_size;
    (void)printed;
    (void)new_str;
    (void)arr;
    (void)root;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}