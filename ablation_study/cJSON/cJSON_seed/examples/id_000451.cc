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
//<ID> 451
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *array = cJSON_Parse("[10]");

    // step 2: Configure
    double first_val = cJSON_GetNumberValue(cJSON_GetArrayItem(array, 0));
    double version_scale = (double)(unsigned char)version[0] / 10.0;
    cJSON *added_number = cJSON_CreateNumber(first_val + version_scale);
    cJSON_AddItemToArray(array, added_number);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(array);
    cJSON *parsed_snapshot = cJSON_Parse(snapshot);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(parsed_snapshot);
    cJSON_Delete(array);
    // API sequence test completed successfully
    return 66;
}