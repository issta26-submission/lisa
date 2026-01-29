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
//<ID> 951
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *arr_key = "outer_array";
    const char *label_key = "label";
    const char *label_val = "compact";
    cJSON *root = (cJSON *)0;
    cJSON *outerArr = (cJSON *)0;
    cJSON *null_item = (cJSON *)0;
    cJSON_bool add_ok = (cJSON_bool)0;
    cJSON *fetched_arr = (cJSON *)0;
    cJSON *fetched_label = (cJSON *)0;
    const char *retrieved_label_str = (const char *)0;
    char *json_str = (char *)0;
    int validation_score = 0;

    // step 2: Setup - create root object, an array and a null item
    root = cJSON_CreateObject();
    outerArr = cJSON_CreateArray();
    null_item = cJSON_CreateNull();

    // step 3: Configure - attach array to root and add the null item into the array
    cJSON_AddItemToObject(root, arr_key, outerArr);
    add_ok = cJSON_AddItemToArray(outerArr, null_item);

    // step 4: Operate - add a label string to the root and retrieve items via GetObjectItem
    cJSON_AddStringToObject(root, label_key, label_val);
    fetched_arr = cJSON_GetObjectItem(root, arr_key);
    fetched_label = cJSON_GetObjectItem(root, label_key);
    retrieved_label_str = cJSON_GetStringValue(fetched_label);

    // step 5: Validate / Serialize - print unformatted and minify then compute a simple score
    json_str = cJSON_PrintUnformatted(root);
    cJSON_Minify(json_str);
    validation_score = (int)(root != (cJSON *)0) + (int)add_ok + (int)(fetched_arr == outerArr) + (int)(fetched_label != (cJSON *)0 && retrieved_label_str != (const char *)0 && retrieved_label_str[0] == 'c') + (int)(json_str != (char *)0);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_free(json_str);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}