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
//<ID> 607
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
    cJSON_AddItemToObject(root, "names", names);

    // step 2: Configure
    int names_size = cJSON_GetArraySize(names);
    cJSON_AddNumberToObject(root, "count", (double)names_size);
    cJSON *flag_true = cJSON_CreateTrue();
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag_true", flag_true);
    cJSON_AddItemToObject(root, "flag_false", flag_false);

    // step 3: Operate & Validate
    cJSON_bool has_names = cJSON_HasObjectItem(root, "names");
    cJSON *got_true = cJSON_GetObjectItem(root, "flag_true");
    cJSON *got_false = cJSON_GetObjectItem(root, "flag_false");
    cJSON_bool is_true = cJSON_IsTrue(got_true);
    cJSON_bool is_false = cJSON_IsFalse(got_false);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON_bool parsed_has_names = cJSON_HasObjectItem(parsed, "names");
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;count=%d;has=%d;parsed_has=%d;istrue=%d;isfalse=%d",
            version ? version : "null",
            names_size,
            (int)has_names,
            (int)parsed_has_names,
            (int)is_true,
            (int)is_false);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}