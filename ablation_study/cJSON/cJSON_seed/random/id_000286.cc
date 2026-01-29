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
//<ID> 286
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *item = (cJSON *)0;
    cJSON *parsed = (cJSON *)0;
    cJSON *parsed_flag = (cJSON *)0;
    cJSON *sum_item = (cJSON *)0;
    char json_buf[] = "  { \"flag\" : false }  ";
    char *printed = (char *)0;
    cJSON_bool add_ref_ok = (cJSON_bool)0;
    cJSON_bool is_false_parsed = (cJSON_bool)0;
    cJSON_bool is_false_created = (cJSON_bool)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    item = cJSON_CreateString("example");
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "example", item);
    cJSON_AddItemToObject(root, "container", arr);

    // step 3: Configure
    add_ref_ok = cJSON_AddItemReferenceToArray(arr, item);
    cJSON_Minify(json_buf);

    // step 4: Operate
    parsed = cJSON_Parse(json_buf);
    parsed_flag = cJSON_GetObjectItem(parsed, "flag");
    is_false_parsed = cJSON_IsFalse(parsed_flag);
    is_false_created = cJSON_IsFalse(cJSON_GetObjectItem(root, "example"));
    sum_item = cJSON_CreateNumber((double)is_false_parsed + (double)is_false_created + (double)add_ref_ok);
    cJSON_AddItemToObject(root, "sum", sum_item);

    // step 5: Validate
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 6: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}