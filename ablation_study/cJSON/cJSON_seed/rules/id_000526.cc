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
//<ID> 526
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    cJSON *root = cJSON_CreateObject();
    cJSON *str_item = cJSON_CreateString("hello_from_item");
    char *printed_heap = NULL;
    cJSON_bool prealloc_ok = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "item_key", str_item);
    cJSON_AddStringToObject(root, "direct_key", "hello_direct");

    // step 3: Operate / Validate
    prealloc_ok = cJSON_PrintPreallocated(root, buffer, 256, 1);
    printed_heap = cJSON_Print(root);
    buffer[0] = (char)('0' + (int)prealloc_ok);
    buffer[1] = ':';
    buffer[2] = printed_heap ? printed_heap[0] : '\0';
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(printed_heap);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}