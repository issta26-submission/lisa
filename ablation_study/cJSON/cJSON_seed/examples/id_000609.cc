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
//<ID> 609
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    const char *names_arr[] = { "alice", "bob", "carol" };
    cJSON *names = cJSON_CreateStringArray(names_arr, 3);
    cJSON *t = cJSON_CreateTrue();
    cJSON *f = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "names", names);
    cJSON_AddItemToObject(root, "flag_true", t);
    cJSON_AddItemToObject(root, "flag_false", f);

    // step 2: Configure
    int names_count = cJSON_GetArraySize(names);
    cJSON_AddNumberToObject(root, "count", (double)names_count);
    cJSON_AddStringToObject(root, "owner", "tester");

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON_bool has_names = cJSON_HasObjectItem(parsed, "names");
    cJSON *parsed_names = cJSON_GetObjectItem(parsed, "names");
    cJSON *first_name_item = cJSON_GetArrayItem(parsed_names, 0);
    const char *first_name = cJSON_GetStringValue(first_name_item);
    cJSON_bool true_check = cJSON_IsTrue(cJSON_GetObjectItem(parsed, "flag_true"));
    cJSON_bool false_check = cJSON_IsFalse(cJSON_GetObjectItem(parsed, "flag_false"));
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;has=%d;count=%d;name=%s;true=%d;false=%d",
            version ? version : "null",
            (int)has_names,
            names_count,
            first_name ? first_name : "null",
            (int)true_check,
            (int)false_check);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}