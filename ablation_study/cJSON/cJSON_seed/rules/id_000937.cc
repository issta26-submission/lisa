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
//<ID> 937
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *first_item = NULL;
    cJSON *second_item = NULL;
    cJSON *retrieved_first = NULL;
    char *json_before = NULL;
    char *json_after = NULL;
    char *buffer = NULL;
    const char *val_first = "alpha";
    const char *val_second = "beta";
    int invalid_flag = 0;
    int bufsize = 128;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    first_item = cJSON_CreateString(val_first);
    cJSON_AddItemToObject(root, "first", first_item);
    second_item = cJSON_AddStringToObject(root, "second", val_second);

    // step 3: Operate / Validate
    retrieved_first = cJSON_GetObjectItem(root, "first");
    invalid_flag = (int)cJSON_IsInvalid(retrieved_first);
    json_before = cJSON_PrintUnformatted(root);
    buffer = (char *)cJSON_malloc((size_t)bufsize);
    memset(buffer, 0, (size_t)bufsize);
    buffer[0] = json_before[0];
    buffer[1] = (char)('0' + (invalid_flag % 10));
    cJSON_DeleteItemFromObject(root, "second");
    json_after = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_free(json_before);
    cJSON_free(json_after);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}