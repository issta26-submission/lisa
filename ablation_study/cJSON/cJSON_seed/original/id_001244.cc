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
//<ID> 1244
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "child", child);

    // step 2: Configure
    cJSON_AddBoolToObject(child, "flag", 1);
    cJSON_AddNumberToObject(child, "num", 123.5);
    cJSON_AddStringToObject(child, "text", "hello");

    // step 3: Operate
    // Use AddItemReferenceToObject to add a reference to the existing child
    cJSON_AddItemReferenceToObject(root, "child_ref", child);
    cJSON *got_child = cJSON_GetObjectItem(root, "child");
    cJSON *got_ref = cJSON_GetObjectItem(root, "child_ref");
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 16);
    memset(buf, 0, printed_len + 16);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);
    cJSON *parsed = cJSON_Parse(buf);
    double extracted = cJSON_GetNumberValue(cJSON_GetObjectItem(cJSON_GetObjectItem(parsed, "child"), "num"));
    cJSON_AddNumberToObject(root, "extracted_num", extracted);

    // step 4: Validate & Cleanup
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    return 66;
}