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
//<ID> 635
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
    cJSON *arr = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON *n3 = cJSON_CreateNumber(3.0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToArray(arr, n3);
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON *note = cJSON_CreateString("original");
    cJSON_AddItemToObject(root, "note", note);
    cJSON *temp = cJSON_CreateString("to_remove");
    cJSON_AddItemToObject(root, "temp", temp);

    // step 2: Configure
    cJSON *dup_arr = cJSON_Duplicate(arr, 1);
    cJSON_AddItemToObject(root, "numbers_copy", dup_arr);
    cJSON_AddNumberToObject(root, "pi", 3.14159);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t snap_len = snapshot ? strlen(snapshot) : 0;
    void *buffer = cJSON_malloc(snap_len + 1);
    memset(buffer, 0, snap_len + 1);
    sprintf((char *)buffer, "%s", snapshot);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_numbers = cJSON_GetObjectItem(parsed, "numbers");
    int parsed_count = cJSON_GetArraySize(parsed_numbers);
    cJSON_DeleteItemFromObject(parsed, "temp");
    cJSON *got_note = cJSON_GetObjectItem(parsed, "note");
    const char *note_str = cJSON_GetStringValue(got_note);
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;count=%d;note=%s;buf=%s",
            version ? version : "null",
            parsed_count,
            note_str ? note_str : "null",
            (char *)buffer);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}