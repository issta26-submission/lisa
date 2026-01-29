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
//<ID> 560
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
    cJSON *orig = cJSON_CreateObject();
    cJSON *num = cJSON_CreateNumber(123.456);
    cJSON_AddItemToObject(orig, "num", num);
    cJSON *raw = cJSON_CreateRaw("{\"rawkey\":true}");

    // step 2: Configure
    cJSON_AddItemReferenceToObject(root, "shared", orig);
    cJSON_AddItemToObject(root, "raw", raw);
    cJSON_AddStringToObject(root, "note", "ref-and-raw");

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_shared = cJSON_GetObjectItem(parsed, "shared");
    cJSON *parsed_num = cJSON_GetObjectItem(parsed_shared, "num");
    double extracted = cJSON_GetNumberValue(parsed_num);
    cJSON *parsed_note_item = cJSON_GetObjectItem(parsed, "note");
    const char *note_str = cJSON_GetStringValue(parsed_note_item);

    // step 4: Cleanup
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;val=%.3f;note=%s",
            version ? version : "null",
            extracted,
            note_str ? note_str : "null");
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    cJSON_Delete(orig);
    // API sequence test completed successfully
    return 66;
}