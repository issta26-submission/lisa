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
//<ID> 1225
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"one\":10,\"two\":20}";
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithOpts(json_text, &parse_end, 1);
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON *one = cJSON_GetObjectItem(parsed, "one");
    cJSON *two = cJSON_GetObjectItem(parsed, "two");
    double val_one = cJSON_GetNumberValue(one);
    double val_two = cJSON_GetNumberValue(two);
    int ints[3];
    ints[0] = (int)val_one;
    ints[1] = (int)val_two;
    ints[2] = (int)(val_one + val_two);
    cJSON *int_array = cJSON_CreateIntArray(ints, 3);
    cJSON_AddItemToObject(root, "ints", int_array);

    // step 3: Operate & Validate
    double sum = val_one + val_two;
    cJSON_AddNumberToObject(root, "sum", sum);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}