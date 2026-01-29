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
//<ID> 1697
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *s0 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(items, s0);
    cJSON *s1 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(items, s1);
    cJSON *s2 = cJSON_CreateString("gamma");
    cJSON_AddItemToArray(items, s2);

    // step 2: Configure
    cJSON_AddStringToObject(meta, "note", "configured");
    cJSON_AddNumberToObject(meta, "count", 3);
    cJSON *child = cJSON_AddObjectToObject(meta, "child");
    cJSON_AddStringToObject(child, "description", "nested");

    // step 3: Operate and Validate
    cJSON_bool has_meta = cJSON_HasObjectItem(root, "meta");
    cJSON *retr_meta = cJSON_GetObjectItem(root, "meta");
    cJSON *retr_note = cJSON_GetObjectItem(retr_meta, "note");
    char *note_val = cJSON_GetStringValue(retr_note);
    int size_before = cJSON_GetArraySize(items);
    char *printed = cJSON_PrintBuffered(root, 256, 1);
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = (char)('0' + (int)has_meta);
    buffer[2] = note_val ? note_val[0] : 'Y';
    buffer[3] = (char)('0' + (int)size_before);

    // step 4: Cleanup
    cJSON_DeleteItemFromArray(items, 1);
    int size_after = cJSON_GetArraySize(items);
    (void)size_after;
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}