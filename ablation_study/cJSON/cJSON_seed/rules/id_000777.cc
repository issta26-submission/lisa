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
//<ID> 777
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *scratch = (char *)cJSON_malloc(256);
    memset(scratch, 0, 256);
    char *printed = NULL;
    char *printed_null = NULL;
    cJSON *root = NULL;
    cJSON *null_item = NULL;
    cJSON *false_item = NULL;
    size_t printed_len = 0;
    size_t null_printed_len = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    null_item = cJSON_AddNullToObject(root, "missing");
    false_item = cJSON_AddFalseToObject(root, "enabled");

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    printed_len = strlen(printed);
    memcpy(scratch, printed, printed_len + 1);
    printed_null = cJSON_PrintUnformatted(null_item);
    null_printed_len = strlen(printed_null);
    memcpy(scratch + printed_len + 1, printed_null, null_printed_len + 1);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(printed_null);
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}