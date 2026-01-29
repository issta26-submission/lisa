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
//<ID> 520
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
    cJSON *name_item = cJSON_CreateString("example_name");
    char *printed_alloc = NULL;
    cJSON_bool prealloc_ok = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "name", name_item);
    cJSON_AddStringToObject(root, "greeting", "hello world");

    // step 3: Operate / Validate
    prealloc_ok = cJSON_PrintPreallocated(root, buffer, 256, 1);
    printed_alloc = cJSON_Print(root);
    size_t printed_len = printed_alloc ? strlen(printed_alloc) : 0;
    size_t copy_len = printed_len > 50 ? 50 : printed_len;
    if (printed_alloc) memcpy(buffer + 2, printed_alloc, copy_len); // single memcpy, no loops
    buffer[0] = (char)('0' + (int)prealloc_ok);
    buffer[1] = ',';

    // step 4: Cleanup
    if (printed_alloc) cJSON_free(printed_alloc);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}