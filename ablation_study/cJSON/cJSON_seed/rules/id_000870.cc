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
//<ID> 870
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *num_item = NULL;
    cJSON *got = NULL;
    cJSON *int_item = NULL;
    char *buffer = NULL;
    size_t buf_size = 128;
    double val = 0.0;
    int truncated = 0;
    cJSON_bool is_number = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(123.456);
    cJSON_AddItemToObject(root, "value", num_item);
    buffer = (char *)cJSON_malloc(buf_size);
    memset(buffer, 0, buf_size);

    // step 3: Operate / Validate
    got = cJSON_GetObjectItem(root, "value");
    val = cJSON_GetNumberValue(got);
    is_number = cJSON_IsNumber(got);
    truncated = (int)val;
    int_item = cJSON_CreateNumber((double)truncated);
    cJSON_AddItemToObject(root, "int_part", int_item);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "value");

    // step 4: Cleanup
    cJSON_free(buffer);
    buffer = NULL;
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)val;
    (void)truncated;
    (void)is_number;
    return 66;
}