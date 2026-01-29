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
//<ID> 1631
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"config\":{\"flag\":false,\"items\":[10,20,30]}}";
    cJSON *root = cJSON_Parse(json);
    cJSON *cfg = cJSON_GetObjectItemCaseSensitive(root, "config");
    cJSON *items = cJSON_GetObjectItemCaseSensitive(cfg, "items");
    cJSON *newbool = cJSON_CreateBool(1);
    cJSON_AddItemToArray(items, newbool);

    // step 2: Configure
    cJSON_AddNumberToObject(cfg, "added", 99.5);
    cJSON *note = cJSON_CreateString("added_note");
    cJSON_AddItemToObject(cfg, "note", note);

    // step 3: Operate and Validate
    int size = cJSON_GetArraySize(items);
    cJSON *second = cJSON_GetArrayItem(items, 1);
    double val = cJSON_GetNumberValue(second);
    cJSON *doubled = cJSON_CreateNumber(val * 2.0);
    cJSON_AddItemToArray(items, doubled);
    char *printed = cJSON_PrintUnformatted(root);
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = printed[0];
    buffer[1] = (char)('0' + (size % 10));
    cJSON *flag = cJSON_GetObjectItemCaseSensitive(cfg, "flag");
    cJSON_bool is_true = cJSON_IsTrue(flag);
    buffer[2] = is_true ? 'T' : 'F';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}