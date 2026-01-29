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
//<ID> 303
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *metrics = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "metrics", metrics);
    cJSON *uptime_item = cJSON_AddNumberToObject(root, "uptime", 1234.5);
    cJSON *requests_item_initial = cJSON_AddNumberToObject(metrics, "requests", 42.0);
    cJSON *errors_item = cJSON_AddNumberToObject(metrics, "errors", 3.0);

    // step 2: Configure
    cJSON *status_item = cJSON_CreateString("ok");
    cJSON_AddItemToObject(root, "status", status_item);

    // step 3: Operate and Validate
    cJSON *metrics_lookup = cJSON_GetObjectItem(root, "metrics");
    cJSON *requests_lookup = cJSON_GetObjectItem(metrics_lookup, "requests");
    double req_count = cJSON_GetNumberValue(requests_lookup);
    char *unformatted = cJSON_PrintUnformatted(root);
    char *pretty = cJSON_Print(root);
    cJSON *next_req = cJSON_CreateNumber(req_count + 1.0);
    cJSON_AddItemToObject(root, "next_request", next_req);

    // step 4: Cleanup
    cJSON_free(unformatted);
    cJSON_free(pretty);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}