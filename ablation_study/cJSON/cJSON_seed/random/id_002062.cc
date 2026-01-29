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
//<ID> 2062
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char buf[64];
    memset(buf, 0, sizeof(buf));
    cJSON *root = NULL;
    cJSON *num_item1 = NULL;
    cJSON *num_item2 = NULL;
    char *json_unformatted = NULL;
    char *json_buffered = NULL;
    double got1 = 0.0;
    double got2 = 0.0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();

    // step 3: Configure
    num_item1 = cJSON_AddNumberToObject(root, "first", 123.0);
    num_item2 = cJSON_AddNumberToObject(root, "second", 456.0);

    // step 4: Operate
    got1 = cJSON_GetNumberValue(num_item1);
    got2 = cJSON_GetNumberValue(num_item2);
    json_unformatted = cJSON_PrintUnformatted(root);
    json_buffered = cJSON_PrintBuffered(root, 128, 0);

    // step 5: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(num_item1 != NULL);
    validation ^= (int)(num_item2 != NULL);
    validation ^= (int)(json_unformatted != NULL);
    validation ^= (int)(json_buffered != NULL);
    validation ^= (int)(got1 == 123.0);
    validation ^= (int)(got2 == 456.0);
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    cJSON_free(json_unformatted);
    cJSON_free(json_buffered);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}