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
//<ID> 773
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *printed = NULL;
    char *buffer = NULL;
    size_t printed_len = 0;
    size_t buffer_len = 0;
    cJSON *root = NULL;
    cJSON *added_false = NULL;
    cJSON *added_null = NULL;
    cJSON *got_false = NULL;
    cJSON *got_null = NULL;
    int is_false = 0;
    int is_null = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    added_false = cJSON_AddFalseToObject(root, "disabled");
    added_null = cJSON_AddNullToObject(root, "value");

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    printed_len = strlen(printed);
    buffer_len = printed_len + 1;
    buffer = (char *)cJSON_malloc(buffer_len);
    memset(buffer, 0, buffer_len);
    memcpy(buffer, printed, printed_len + 1);
    got_false = cJSON_GetObjectItem(root, "disabled");
    got_null = cJSON_GetObjectItem(root, "value");
    is_false = cJSON_IsFalse(got_false);
    is_null = cJSON_IsNull(got_null);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}