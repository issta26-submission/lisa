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
//<ID> 603
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
    const char *const names_arr[] = { "alice", "bob", "carol" };
    cJSON *names = cJSON_CreateStringArray(names_arr, 3);
    cJSON_AddItemToObject(root, "names", names);

    // step 2: Configure
    cJSON *flag_true = cJSON_CreateTrue();
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "ok", flag_true);
    cJSON_AddItemToObject(root, "bad", flag_false);
    cJSON_AddNumberToObject(root, "count", (double)cJSON_GetArraySize(names));

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON_bool has_names = cJSON_HasObjectItem(parsed, "names");
    cJSON *parsed_ok = cJSON_GetObjectItem(parsed, "ok");
    cJSON *parsed_bad = cJSON_GetObjectItem(parsed, "bad");
    cJSON_bool is_ok_true = cJSON_IsTrue(parsed_ok);
    cJSON_bool is_bad_false = cJSON_IsFalse(parsed_bad);
    cJSON *parsed_names = cJSON_GetObjectItem(parsed, "names");
    cJSON *first = cJSON_GetArrayItem(parsed_names, 0);
    const char *first_name = cJSON_GetStringValue(first);

    // step 4: Cleanup
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;has=%d;ok=%d;bad=%d;first=%s",
            version ? version : "null",
            (int)has_names,
            (int)is_ok_true,
            (int)is_bad_false,
            first_name ? first_name : "null");
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}