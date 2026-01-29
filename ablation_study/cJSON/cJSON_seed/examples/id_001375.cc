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
//<ID> 1375
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *message_item = cJSON_CreateString("initial");
    cJSON_AddItemToObject(root, "message", message_item);
    cJSON *count_item = cJSON_AddNumberToObject(root, "count", 3.0);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *note = cJSON_CreateString("note");
    cJSON_AddItemToObject(meta, "note", note);
    int buf_len = 512;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);

    // step 3: Operate & Validate
    cJSON *msg_ptr = cJSON_GetObjectItem(root, "message");
    cJSON_bool was_null = cJSON_IsNull(msg_ptr);
    double current = cJSON_GetNumberValue(count_item);
    cJSON_SetNumberHelper(count_item, current + (double)was_null);
    cJSON *replacement = cJSON_CreateString("replaced");
    cJSON_ReplaceItemViaPointer(root, msg_ptr, replacement);
    cJSON_PrintPreallocated(root, buffer, buf_len, 1);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free(buffer);
    // API sequence test completed successfully
    return 66;
}