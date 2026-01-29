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
//<ID> 726
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    char *printed = NULL;
    size_t printed_len = 0;
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *flag_orig = cJSON_CreateTrue();
    cJSON *num_orig = cJSON_CreateNumber(7.0);
    cJSON *flag_repl = NULL;
    cJSON *num_repl = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToObject(child, "number", num_orig);
    cJSON_AddItemToObject(root, "flag", flag_orig);

    // step 3: Operate / Validate
    flag_repl = cJSON_CreateTrue();
    cJSON_ReplaceItemInObjectCaseSensitive(root, "flag", flag_repl);
    num_repl = cJSON_CreateNumber(42.0);
    cJSON_ReplaceItemViaPointer(child, num_orig, num_repl);
    printed = cJSON_PrintUnformatted(root);
    printed_len = printed ? strlen(printed) : 0;
    memcpy(buffer, printed ? printed : "", printed_len);
    buffer[printed_len] = '\0';
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}