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
//<ID> 1925
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
    cJSON_AddNullToObject(root, "meta");

    // step 2: Configure
    cJSON *answer = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "answer", answer);

    // step 3: Operate and Validate
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, buffer, buflen, 1);
    cJSON *got_answer = cJSON_GetObjectItem(root, "answer");
    double answer_value = cJSON_GetNumberValue(got_answer);
    buffer[0] = (char)('0' + ((int)answer_value % 10));
    buffer[1] = (char)('0' + (printed_ok % 10));
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}