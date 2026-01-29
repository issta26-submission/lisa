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
//<ID> 939
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"existing\": 123}";
    size_t initial_len = sizeof(initial_json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(initial_json, initial_len, &parse_end, 1);

    // step 2: Configure
    float values[] = {1.5f, 2.5f, 3.25f};
    cJSON *numbers = cJSON_CreateFloatArray(values, 3);
    cJSON_AddItemToObjectCS(root, "numbers", numbers);
    cJSON *label = cJSON_CreateString("sample");
    cJSON_AddItemToObjectCS(root, "label", label);

    // step 3: Operate
    cJSON *numbers_ref = cJSON_CreateArrayReference(numbers);
    cJSON_AddItemToObjectCS(root, "numbers_ref", numbers_ref);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    void *buf = cJSON_malloc(256);
    memset(buf, 0, 256);
    cJSON_PrintPreallocated(root, (char *)buf, 256, 1);
    cJSON_free(out);
    cJSON_free(buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}