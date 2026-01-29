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
//<ID> 285
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
    cJSON *false_item = (cJSON *)0;
    cJSON *parsed = (cJSON *)0;
    cJSON *parsed_val = (cJSON *)0;
    cJSON *num_parsed = (cJSON *)0;
    cJSON *num_original = (cJSON *)0;
    cJSON_bool is_false_parsed = (cJSON_bool)0;
    cJSON_bool is_false_original = (cJSON_bool)0;
    char json_buf[] = "  { \"k\" : false }  ";

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "myArray", arr);

    // step 3: Configure
    cJSON_AddItemReferenceToArray(arr, false_item);
    cJSON_Minify(json_buf);

    // step 4: Operate
    parsed = cJSON_Parse(json_buf);
    parsed_val = cJSON_GetObjectItem(parsed, "k");
    is_false_parsed = cJSON_IsFalse(parsed_val);
    is_false_original = cJSON_IsFalse(cJSON_GetArrayItem(arr, 0));

    // step 5: Validate
    num_parsed = cJSON_CreateNumber((double)is_false_parsed);
    num_original = cJSON_CreateNumber((double)is_false_original);
    cJSON_AddItemToObject(root, "parsedFalse", num_parsed);
    cJSON_AddItemToObject(root, "originalFalse", num_original);

    // step 6: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}