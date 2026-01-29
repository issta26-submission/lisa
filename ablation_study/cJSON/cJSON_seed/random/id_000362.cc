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
//<ID> 362
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *raw_item = (cJSON *)0;
    cJSON *got_num = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    double extracted_number = 0.0;
    const char *raw_json = "{\"inner\":123}";

    // step 2: Initialize
    root = cJSON_CreateObject();

    // step 3: Configure
    num_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "num", num_item);
    raw_item = cJSON_AddRawToObject(root, "raw", raw_json);

    // step 4: Operate
    got_num = cJSON_GetObjectItem(root, "num");
    detached = cJSON_DetachItemFromObject(root, "raw");
    cJSON_AddItemToObject(root, "raw_detached", detached);

    // step 5: Validate
    extracted_number = cJSON_GetNumberValue(got_num);
    (void)extracted_number;
    (void)raw_item;

    // step 6: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}