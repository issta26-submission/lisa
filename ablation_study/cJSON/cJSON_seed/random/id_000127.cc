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
//<ID> 127
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *description = (cJSON *)0;
    cJSON *values = (cJSON *)0;
    char *json_text = (char *)0;
    float numbers[3];
    int count = 3;
    double validated_count = 0.0;

    // step 2: Initialize - create root object, a string node, and a float array
    root = cJSON_CreateObject();
    description = cJSON_CreateString("An example object containing float values");
    numbers[0] = 1.0f;
    numbers[1] = 2.5f;
    numbers[2] = -3.25f;
    values = cJSON_CreateFloatArray(numbers, count);

    // step 3: Configure - attach a title via AddStringToObject, attach description and values, add a numeric count
    cJSON_AddStringToObject(root, "title", "Sample Title");
    cJSON_AddItemToObject(root, "description", description);
    cJSON_AddItemToObject(root, "values", values);
    cJSON_AddNumberToObject(root, "count", (double)count);

    // step 4: Operate - read back the count and record it as validated_count, then add it to the object
    validated_count = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "count"));
    cJSON_AddNumberToObject(root, "validated_count", validated_count);

    // step 5: Validate - produce JSON text and free it
    json_text = cJSON_PrintUnformatted(root);
    cJSON_free(json_text);

    // step 6: Cleanup
    cJSON_Delete(root);

    return 66;
    // API sequence test completed successfully
}