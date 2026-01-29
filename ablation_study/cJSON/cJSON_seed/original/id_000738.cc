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
//<ID> 738
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"title\":\"test\",\"numbers\":[1,2]}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json_text, &parse_end, 1);

    // step 2: Configure
    cJSON *numbers = cJSON_GetObjectItem(root, "numbers");
    cJSON *inserted_number = cJSON_CreateNumber(99.0);
    cJSON_InsertItemInArray(numbers, 1, inserted_number);
    const char *labels_array[] = { "alpha", "beta", "gamma" };
    cJSON *labels = cJSON_CreateStringArray(labels_array, 3);
    cJSON_AddItemToObject(root, "labels", labels);
    cJSON *answer = cJSON_CreateNumber(0.0);
    cJSON_SetNumberHelper(answer, 42.0);
    cJSON_AddItemToObject(root, "answer", answer);

    // step 3: Operate
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}