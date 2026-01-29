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
//<ID> 1920
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_AddItemToArray(items, true_item);
    cJSON *num_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(items, num_item);

    // step 2: Configure
    cJSON_AddNullToObject(root, "optional");
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *id_str = cJSON_CreateString("id_123");
    cJSON_AddItemToObject(meta, "id", id_str);

    // step 3: Operate
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, buffer, 256, 1);
    char first_char = buffer[0];
    cJSON_AddBoolToObject(meta, "print_ok", printed_ok);
    cJSON_AddNumberToObject(meta, "printed_first_char", (double)(unsigned char)first_char);
    cJSON_free(buffer);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}