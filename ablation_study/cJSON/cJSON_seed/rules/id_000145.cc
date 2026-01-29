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
//<ID> 145
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char json[] = "{\"val\":123.5,\"flag\":false}";
    size_t len = sizeof(json) - 1;
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithLengthOpts(json, len, &parse_end, 1);
    cJSON *constructed = cJSON_CreateObject();
    cJSON *val_item = NULL;
    double extracted = 0.0;
    cJSON_bool equal = 0;

    // step 2: Setup / Configure
    cJSON_AddNumberToObject(constructed, "val", 123.5);
    cJSON_AddFalseToObject(constructed, "flag");

    // step 3: Operate / Validate
    val_item = cJSON_GetObjectItem(parsed, "val");
    extracted = cJSON_GetNumberValue(val_item);
    equal = cJSON_Compare(parsed, constructed, 1);

    // step 4: Cleanup
    cJSON_Delete(constructed);
    cJSON_Delete(parsed);
    // API sequence test completed successfully
    return 66;
}